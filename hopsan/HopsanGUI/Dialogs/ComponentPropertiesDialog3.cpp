/*-----------------------------------------------------------------------------

 Copyright 2017 Hopsan Group

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

 The full license is available in the file GPLv3.
 For details about the 'Hopsan Group' or information about Authors and
 Contributors see the HOPSANGROUP and AUTHORS files that are located in
 the Hopsan source code root directory.

-----------------------------------------------------------------------------*/

//!
//! @file   ComponentPropertiesDialog3.cpp
//! @author Peter Nordin <peter.nordin@liu.se>
//! @date   2010-03-01
//!
//! @brief Contains a dialog class for changing component properties
//!
//$Id$

//Qt includes
#include <QDebug>
#include <QMessageBox>
#include <QScrollArea>
#include <QApplication>
#include <QDesktopWidget>
#include <QTableView>
#include <QHeaderView>
#include <QScrollBar>
#include <QMenu>
#include <QMainWindow>
#include <QLineEdit>
#include <QGroupBox>
#include <QToolButton>
#include <QFileDialog>
#include <QTextEdit>


//Hopsan includes
#include "common.h"
#include "global.h"
#include "ComponentPropertiesDialog3.h"
#include "Configuration.h"
#include "DesktopHandler.h"
#include "Dialogs/MovePortsDialog.h"
#include "GUIObjects/GUIComponent.h"
#include "GUIObjects/GUIContainerObject.h"
#include "GUIPort.h"
#include "UndoStack.h"
#include "Utilities/GUIUtilities.h"
#include "Utilities/HighlightingUtilities.h"
#include "Utilities/WebviewWrapper.h"
#include "LibraryHandler.h"
#include "Widgets/ModelWidget.h"
#include "Widgets/SystemParametersWidget.h"
#include "LibraryHandler.h"
#include "Widgets/LibraryWidget.h"
#include "MessageHandler.h"
#include "ModelHandler.h"

#ifdef USEDISCOUNT
extern "C" {
#include "mkdio.h"
}
#endif

//! @class ComponentPropertiesDialog3
//! @brief The ComponentPropertiesDialog3 class is a Widget used to interact with component parameters.
//!
//! It reads and writes parameters to the core components.
//!


// Help functions
inline bool isPathAbsolute(const QString &path)
{
    QFileInfo fi(path);
    return fi.isAbsolute();
}

class ValueEdit : public QLineEdit
{
    Q_OBJECT
public:
    ValueEdit(QWidget * parent = nullptr) :
        QLineEdit(parent)
    { }

public slots:
    void updateToolTip(QString tt)
    {
        setToolTip(tt);
    }
};

//! @brief Constructor for the parameter dialog for components
//! @param pModelObject Pointer to the component
//! @param parent Pointer to the parent widget
ComponentPropertiesDialog3::ComponentPropertiesDialog3(ModelObject *pModelObject, QWidget *pParent)
    : QDialog(pParent)
{
    mpModelObject = pModelObject;
    if (mpModelObject)
    {
        mAllowEditing = (!mpModelObject->isLocallyLocked() && (mpModelObject->getModelLockLevel() < FullyLocked));
    }

    this->setPalette(gpConfig->getPalette());
    setWindowTitle(tr("Component Properties"));
    createEditStuff();

    connect(this, SIGNAL(setLimitedModelEditingLock(bool)), mpModelObject->getParentSystemObject()->mpModelWidget, SLOT(lockModelEditingLimited(bool)));

    // Lock model for changes
    emit setLimitedModelEditingLock(true);
}

void ComponentPropertiesDialog3::showEvent(QShowEvent *event)
{
    //For FMI wrappers the help path may change after the component
    //is created, so it must update every time we open the dialog.
    if(mpModelObject->getTypeName() == "FMIWrapper" || mpModelObject->getTypeName() == "FMIWrapperQ") {
        createHelpWidget();
    }

    QDialog::showEvent(event);
}


//! @brief Verifies that a parameter value does not begin with a number but still contains illegal characters.
//! @note This is a temporary solution. It shall be removed when parsing equations as parameters works.
bool VariableTableWidget::cleanAndVerifyParameterValue(QString &rValue, const QString type)
{
    //! @todo I think CORE should handle all of this stuff
    QString tempVal = rValue;
    QStringList selfParameterNames = mpModelObject->getParameterNames();
    QStringList allAccessibleParentSystemParamNames = getAllAccessibleSystemParameterNames(mpModelObject->getParentSystemObject());
    QString error;

    if(verifyParameterValue(tempVal, type, selfParameterNames, allAccessibleParentSystemParamNames, error))
    {
        // Set corrected text
        rValue = tempVal;
        return true;
    }
    else
    {
        QMessageBox::critical(gpMainWindowWidget, "Error", error.append(" Resetting parameter value!"));
        return false;
    }
}

bool VariableTableWidget::setAliasName(const int row)
{
    // Skip separator rows
    if (columnSpan(row,0)>2)
    {
        return true;
    }

    QString alias = item(row,VariableTableWidget::Alias)->text();
    //! @todo since alias=empty means unregister we can not skip it, but there should be some check if a variable has changed, so that we do not need to go through set for all variables every freaking time
    QString name = item(row,VariableTableWidget::Name)->toolTip();
    QStringList parts = name.split("#");
    if (parts.size() == 2)
    {
        //! @todo look over this name stuff
        mpModelObject->getParentSystemObject()->setVariableAlias(mpModelObject->getName()+"#"+parts[0]+"#"+parts[1], alias);
    }
    return true;
}


//! @brief Reads the values from the dialog and writes them into the core component
void ComponentPropertiesDialog3::okPressed()
{
    setName();
    setAliasNames();
    setSystemProperties();
    if (setVariableValues())
    {
        close();
    }
}

void ComponentPropertiesDialog3::applyAndSimulatePressed()
{
    applyPressed();
    mpModelObject->getParentSystemObject()->mpModelWidget->simulate_blocking();
}


void ComponentPropertiesDialog3::applyPressed()
{
    setName();
    setAliasNames();
    setSystemProperties();
    setVariableValues();
}

void ComponentPropertiesDialog3::openSourceCode()
{
    auto appearance = gpLibraryHandler->getModelObjectAppearancePtr(mpModelObject->getTypeName());
    QString basePath = appearance->getBasePath();
    if(!basePath.isEmpty()) {
        basePath.append("/");
    }
    QString fileName = appearance->getSourceCodeFile();
    gpModelHandler->loadTextFile(basePath+fileName);
}


void ComponentPropertiesDialog3::editPortPos()
{
    auto *pDialog = new MovePortsDialog(mpModelObject, mpModelObject->getParentSystemObject()->getGfxType(), this);
    connect(pDialog, SIGNAL(finished()), mpModelObject, SLOT(refreshExternalPortsAppearanceAndPosition()), Qt::UniqueConnection);
    connect(pDialog, &MovePortsDialog::finished, mpVariableTableWidget, &VariableTableWidget::refreshInternalPortInfo);
}


bool ComponentPropertiesDialog3::setAliasNames()
{
    return mpVariableTableWidget->setAliasNames();
}


//! @brief Sets the parameters and start values in the core component. Read the values from the dialog and write them into the core component.
bool ComponentPropertiesDialog3::setVariableValues()
{
    bool isOK = mpVariableTableWidget->setStartValues();
    //isOK *= mpVariableTableWidget->setCustomPlotScaleValues();
    return isOK;
}

void ComponentPropertiesDialog3::setName()
{
    mpModelObject->getParentSystemObject()->renameModelObject(mpModelObject->getName(), mpNameEdit->text());
}

void ComponentPropertiesDialog3::setSystemProperties()
{
    if (mpSystemProperties)
    {
        mpSystemProperties->setValues();
    }
}

void ComponentPropertiesDialog3::closeEvent(QCloseEvent* event)
{
    emit setLimitedModelEditingLock(false);
    QWidget::closeEvent(event);
}

void ComponentPropertiesDialog3::reject()
{
    QDialog::reject();
    QDialog::close();
}

void ComponentPropertiesDialog3::openDescription()
{
    QMainWindow *pDescriptionWindow = new QMainWindow(gpMainWindowWidget);
    pDescriptionWindow->setCentralWidget(createHelpWidget());
    pDescriptionWindow->setWindowTitle(gpLibraryHandler->getModelObjectAppearancePtr(mpModelObject->getTypeName())->getDisplayName());
    pDescriptionWindow->resize(this->size());
    pDescriptionWindow->show();
}


//! @brief Applies the parameter set selected in the drop-down menu by reading its SSV file
void ComponentPropertiesDialog3::applyParameterSet()
{
    //Read from SSV file (XML)
    QString fileName = mpModelObject->getParameterSets().value(mpSetsComboBox->currentText());
    QList<SsvParameter> ssvParameters;
    readFromSsv(mpModelObject->getAppearanceData()->getBasePath()+"/"+fileName, ssvParameters);

    for(const auto &ssvParameter : ssvParameters) {
        if(!mpModelObject->hasParameter(ssvParameter.name)) {
            gpMessageHandler->addWarningMessage("Parameter not found in component: "+ssvParameter.name+" (ignored)");
            continue;
        }
        CoreParameterData parameter;
        mpModelObject->getParameter(ssvParameter.name, parameter);
        QString hopsanUnit = mpVariableTableWidget->getSelectedUnit(ssvParameter.name);
        if(ssvParameter.dataType == ssv::datatype::real) {
            if(hopsanUnit == ssvParameter.unit) {
                this->mpVariableTableWidget->setValue(ssvParameter.name, ssvParameter.value);
            }
            else {
                QStringList quantities = gpConfig->getQuantitiesForUnit(ssvParameter.unit);
                if(quantities.isEmpty()) {
                    gpMessageHandler->addWarningMessage("Unit is not supported by Hopsan: "+ssvParameter.unit+". Setting parameter without unit conversion.");
                    this->mpVariableTableWidget->setValue(ssvParameter.name, ssvParameter.value);
                }
                else {
                    QString ssvQuantity, hopsanQuantity;
                    ssvQuantity = quantities[0];    //Guessing for first quantity is the best we can do
                    QStringList hopsanQuantities = gpConfig->getQuantitiesForUnit(hopsanUnit);
                    if(parameter.mQuantity.isEmpty() && !hopsanQuantities.isEmpty()) {
                        hopsanQuantity = hopsanQuantities[0];
                    }
                    else {
                        hopsanQuantity = parameter.mQuantity;
                    }
                    UnitConverter uc1 = gpConfig->getUnitScaleUC(ssvQuantity, ssvParameter.unit);         //Converts between SSV unit and base unit
                    UnitConverter uc2 = gpConfig->getUnitScaleUC(hopsanQuantity, hopsanUnit); //Converts between Hopsan unit and base unit
                    this->mpVariableTableWidget->setValue(ssvParameter.name, uc2.convertFromBase(uc1.convertToBase(ssvParameter.value)));
                }
            }
        }
        else {
            this->mpVariableTableWidget->setValue(ssvParameter.name, ssvParameter.value);
        }
    }
}

QGridLayout* ComponentPropertiesDialog3::createNameAndTypeEdit()
{
    QGridLayout *pNameTypeLayout = new QGridLayout();
    QLabel *pNameLabel = new QLabel("Name: ");
    mpNameEdit = new QLineEdit(mpModelObject->getName());
    QLabel *pCQSTypeLabel = new QLabel("CQS Type: \""+mpModelObject->getTypeCQS()+"\"");
    QLabel *pTypeNameLabel = new QLabel("Typename: \""+mpModelObject->getTypeName()+"\"");
    int r=0;
    pNameTypeLayout->addWidget(pNameLabel,r,0);
    pNameTypeLayout->addWidget(mpNameEdit,r,1);
    r++;
    pNameTypeLayout->addWidget(pTypeNameLabel,r,0,1,2);
    r++;
    if (!mpModelObject->getSubTypeName().isEmpty())
    {
        QLabel *pSubTypeNameLabel = new QLabel("SubTypename: \""+mpModelObject->getSubTypeName()+"\"");
        pNameTypeLayout->addWidget(pSubTypeNameLabel,r,0,1,2);
        r++;
    }
    pNameTypeLayout->addWidget(pCQSTypeLabel,r,0,1,2);

    return pNameTypeLayout;
}

QDialogButtonBox *ComponentPropertiesDialog3::createOKButtonBox()
{
    QPushButton *pCancelButton = new QPushButton(tr("&Cancel"), this);
    QPushButton *pOkButton = new QPushButton(tr("&Ok"), this);
    QPushButton *pApplyButton = new QPushButton(tr("&Apply"), this);
    QPushButton *pApplyAndSimulateButton = new QPushButton(tr("Apply && &Simulate"), this);
    QDialogButtonBox *pButtonBox = new QDialogButtonBox(Qt::Horizontal, this);
    QString filePath = mpModelObject->getAppearanceData()->getSourceCodeFile();
    if(!filePath.isEmpty())
    {
        QPushButton *pSourceCodeButton = new QPushButton(tr("&Source Code"), this);
        pButtonBox->addButton(pSourceCodeButton, QDialogButtonBox::ActionRole);
        connect(pSourceCodeButton, SIGNAL(clicked()), this, SLOT(openSourceCode()));
    }
    pButtonBox->addButton(pApplyAndSimulateButton, QDialogButtonBox::ActionRole);
    pButtonBox->addButton(pApplyButton, QDialogButtonBox::ActionRole);
    pButtonBox->addButton(pOkButton, QDialogButtonBox::ActionRole);
    pButtonBox->addButton(pCancelButton, QDialogButtonBox::ActionRole);
    connect(pApplyAndSimulateButton, SIGNAL(clicked()), this, SLOT(applyAndSimulatePressed()));
    connect(pApplyButton, SIGNAL(clicked()), this, SLOT(applyPressed()));
    connect(pOkButton, SIGNAL(clicked()), this, SLOT(okPressed()));
    connect(pCancelButton, SIGNAL(clicked()), this, SLOT(close()));
    pApplyButton->setEnabled(mAllowEditing);
    pOkButton->setEnabled(mAllowEditing);
    pOkButton->setDefault(true);

    return pButtonBox;
}

QWidget *ComponentPropertiesDialog3::createHelpWidget()
{
    if(!mpModelObject->getHelpText().isEmpty() || !mpModelObject->getHelpPicture().isEmpty() || !mpModelObject->getHelpLinks().isEmpty() || !mpModelObject->getHelpHtmlPath().isEmpty() || mpModelObject->getTypeName() == "FMIWrapper" || mpModelObject->getTypeName() == "FMIWrapperQ")
    {
        QScrollArea *pHelpScrollArea = new QScrollArea();
        QGroupBox *pHelpWidget = new QGroupBox();
        QVBoxLayout *pHelpLayout = new QVBoxLayout(pHelpWidget);

        if(mpModelObject->getTypeName() != "FMIWrapper" && mpModelObject->getTypeName() != "FMIWrapperQ")    //No point in showing component type name as heading for FMI wrappers
        {
            QLabel *pHelpHeading = new QLabel(gpLibraryHandler->getModelObjectAppearancePtr(mpModelObject->getTypeName())->getDisplayName());
            pHelpHeading->setAlignment(Qt::AlignLeft);
            QFont tempFont = pHelpHeading->font();
            tempFont.setPixelSize(16);
            tempFont.setBold(true);
            pHelpHeading->setFont(tempFont);
            pHelpLayout->addWidget(pHelpHeading);
        }

        if(!mpModelObject->getHelpHtmlPath().isEmpty() || ((mpModelObject->getTypeName() == "FMIWrapper" || mpModelObject->getTypeName() == "FMIWrapperQ") && mpModelObject->hasParameter("temppath")))
        {
            WebViewWrapper* pHtmlView = new WebViewWrapper(false);
            QString path = mpModelObject->getAppearanceData()->getBasePath() + mpModelObject->getHelpHtmlPath();
            if (path.endsWith(".md"))
            {
#ifdef USEDISCOUNT
                QFileInfo mdFile(path);

                QString htmlFilePath = mpModelObject->getHelpHtmlPath();
                htmlFilePath.truncate(htmlFilePath.size()-3);
                htmlFilePath.append(".html");
                htmlFilePath.prepend(gpDesktopHandler->getTempPath()+"html/");

                // Make sure temp dir exist
                QFileInfo fi(htmlFilePath);
                QDir().mkpath(fi.absolutePath());

                FILE *inFile = fopen(path.toStdString().c_str(), "r");
                FILE *outFile = fopen(htmlFilePath.toStdString().c_str(), "w");
                if (inFile && outFile)
                {
                    // Generate html from markdown
                    MMIOT *doc = mkd_in(inFile, 0);
                    int rc;// = markdown(doc, outFile, MKD_TABSTOP);
                    if (rc == -1) {
                        // If rc == 0 then mkd_cleanup has already ben run on doc
                        mkd_cleanup(doc);
                    }
                    fclose(inFile);
                    fclose(outFile);

                    // Now parse the html file for equations and images
                    QFile htmlFile(htmlFilePath);
                    htmlFile.open(QIODevice::ReadOnly);
                    QTextStream htmlStream(&htmlFile);
                    QString tmp_html = htmlStream.readAll();
                    htmlFile.close();

                    struct EquationData {
                        size_t lineNumber = 0;
                        QString label;
                        size_t number;
                        QString equation;
                        QString htmlequation;
                    };

                    QMap<size_t, EquationData> equationMap;
                    QMap<size_t, QString> imageMap;
                    QMap<QString, QString> eqrefMap;
                    QTextStream inHtmlStream(&tmp_html);
                    size_t lineNumber = 0;
                    size_t equationNumber = 1;
                    while(!inHtmlStream.atEnd())
                    {
                        //! @todo multiline equations?
                        QString line = inHtmlStream.readLine().trimmed();
                        if (line.startsWith("<!---EQUATION"))
                        {
                            EquationData eqdata;
                            eqdata.lineNumber = lineNumber;
                            eqdata.number = equationNumber++;
                            if (line.contains("LABEL")) {
                                QRegularExpression re(R"(LABEL=(\S+)\s+(.+)--->)");
                                QRegularExpressionMatch match = re.match(line, 13);
                                QString label = match.captured(1).trimmed();
                                if (!label.isEmpty()) {
                                    eqdata.label = label;
                                }
                                eqdata.equation = match.captured(2).trimmed();
                            }
                            else {
                                QRegularExpression re(R"(\s*(.+)--->)");
                                eqdata.equation = re.match(line, 13).captured(1).trimmed();
                            }
                            eqdata.htmlequation = QString("<div class=\"equation\" expr=\"\\tag{%1} %2\"></div>").arg(eqdata.number).arg(eqdata.equation);
                            equationMap.insert(lineNumber, eqdata);
                        }
                        else if (line.startsWith("<p><img"))
                        {
                            size_t b = line.indexOf("src=")+4;
                            QString image = line.left(b+1)+"file:///"+mdFile.absolutePath()+"/"+line.right(line.size()-b-1);
                            imageMap.insert( lineNumber, image );
                        }
                        else if (line.contains("EQREF")) {
                            QRegularExpression re(R"(EQREF{(\S+)})");
                            auto matchIterator = re.globalMatch(line);
                            while (matchIterator.hasNext()) {
                                auto match = matchIterator.next();
                                auto label = match.captured(1);
                                eqrefMap.insert(QString("EQREF{%1}").arg(label), label);
                            }
                        }
                        ++lineNumber;
                    }

                    // Resolve equation references, (replace the label with the actual equation number)
                    const auto allEquations = equationMap.values();
                    for (auto it = eqrefMap.begin(); it != eqrefMap.end(); ++it) {
                        QString& rLabel = it.value();

                        auto findEquationWithLabel = [rLabel](const auto& equationData) -> bool {
                            return (equationData.label == rLabel);
                        };
                        auto eq_it = std::find_if(equationMap.begin(), equationMap.end(), findEquationWithLabel);
                        if (eq_it != equationMap.end()) {
                            rLabel = QString("(%1)").arg(eq_it.value().number);
                        }
                    }

                    htmlFile.open(QIODevice::WriteOnly);
                    QTextStream& outHtmlStream = htmlStream;
                    outHtmlStream.seek(0);

                    auto processHtmlStream = [&imageMap, &equationMap, &eqrefMap, &inHtmlStream, &outHtmlStream]() {
                        inHtmlStream.seek(0);
                        size_t lineNumber = 0;
                        while (!inHtmlStream.atEnd())
                        {
                            // Check if image line should be replaced
                            auto imit = imageMap.find(lineNumber);
                            if (imit != imageMap.end())
                            {
                                outHtmlStream << imit.value() << "\n";
                                imageMap.erase(imit);
                                // Also discard the actual line
                                inHtmlStream.readLine();
                            }
                            // If not then print the actual line (with replaced equation references)
                            else
                            {
                                //! @todo Could be slightly more efficient if remembering what lines various equation refs are
                                QString line = inHtmlStream.readLine();
                                for (auto it = eqrefMap.begin(); it != eqrefMap.end(); ++ it) {
                                    line.replace(it.key(), it.value());
                                }
                                outHtmlStream << line << "\n";
                            }

                            // Check if an equation should be added
                            auto eqit = equationMap.find(lineNumber);
                            if (eqit != equationMap.end())
                            {
                                outHtmlStream << eqit.value().htmlequation << "\n";
                                equationMap.erase(eqit);
                            }

                            lineNumber++;
                        }
                    };

                    if (!equationMap.empty()) {
                        htmlStream << "<!DOCTYPE html>\n";
                        htmlStream << "<html>\n";
                        htmlStream << "  <head>\n";
                        htmlStream << "    <meta charset=\"UTF-8\">\n";
                        htmlStream << "      " << QString("<script src=\"file:///%1katex/katex.min.js\" type=\"text/javascript\"></script>\n").arg(gpDesktopHandler->getExecPath()+"../dependencies/");
                        htmlStream << "      " << QString("<link href=\"file:///%1katex/katex.min.css\" rel=\"stylesheet\" type=\"text/css\">\n").arg(gpDesktopHandler->getExecPath()+"../dependencies/");
                        htmlStream << "  </head>\n";

                        processHtmlStream();

                        htmlStream << "  <script type=\"text/javascript\">\n";
                        htmlStream << "    var eqns = document.getElementsByClassName(\"equation\");\n";
                        htmlStream << "    Array.prototype.forEach.call(eqns, function(eq) {\n";
                        htmlStream << "      katex.render(eq.getAttribute(\"expr\"), eq, { displayMode: true });\n";
                        htmlStream << "    });\n";
                        htmlStream << "  </script>\n";

                        htmlStream << "</html>\n";

                    }
                    else {
                        processHtmlStream();
                    }

                    htmlFile.close();

                    // Set html to view
                    pHtmlView->loadHtmlFile(QUrl::fromLocalFile(htmlFilePath));
                }
                else
                {
                    pHtmlView->showText(QString("Error: Could not convert %1 to HTML or load the file.").arg(path));
                }
#else
                pHtmlView->showText(QString("Error: Markdown support is not available in this build!").arg(path));
#endif
            }
            else
            {
                pHtmlView->loadHtmlFile(QUrl::fromLocalFile(path));
            }
            pHelpLayout->addWidget(pHtmlView);
            pHtmlView->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        }

        if(!mpModelObject->getHelpPicture().isEmpty())
        {
            QLabel *pHelpPicture = new QLabel();
            QPixmap helpPixMap(mpModelObject->getAppearanceData()->getBasePath() + mpModelObject->getHelpPicture());
            pHelpPicture->setPixmap(helpPixMap);
            pHelpLayout->addWidget(pHelpPicture);
            pHelpPicture->setAlignment(Qt::AlignLeft);
        }

        if(!mpModelObject->getHelpText().isEmpty())
        {
            QLabel *pHelpText = new QLabel(mpModelObject->getHelpText(), this);
            pHelpText->setWordWrap(true);
            pHelpLayout->addWidget(pHelpText);
            pHelpText->setAlignment(Qt::AlignLeft);
        }

        const QStringList &links = mpModelObject->getHelpLinks();
        for (auto &link : links)
        {
            QString path = mpModelObject->getAppearanceData()->getBasePath()+link;
            QString linkstr;
            QFileInfo fi(path);
            if (fi.isFile())
            {
                linkstr = QString("External document: <a href=\"file:///%1\">%2</a>").arg(path).arg(link);
            }
            else
            {
                QString link2 = link;
                if (!(link2.startsWith("http://") || link2.startsWith("https://")))
                {
                    link2.prepend("http://");
                }
                linkstr = QString("External document: <a href=\"%1\">%2</a>").arg(link2).arg(link2);
            }
            QLabel *pHelpLink = new QLabel(linkstr,this);
            pHelpLink->setOpenExternalLinks(true);
            pHelpLayout->addWidget(pHelpLink);
            pHelpLink->setAlignment(Qt::AlignLeft);
        }

        // Add dummy stretch widget at bottom if no html is present (to force image / text / link together)
        if (mpModelObject->getHelpHtmlPath().isEmpty())
        {
            QWidget *pDummyWidget = new QWidget(this);
            pHelpLayout->addWidget(pDummyWidget, 1);
        }

        pHelpWidget->setStyleSheet(QString::fromUtf8("QGroupBox {background-color: white; border: 2px solid gray; border-radius: 5px;}"));

        pHelpScrollArea->setWidget(pHelpWidget);
        pHelpScrollArea->setWidgetResizable(true);
        pHelpScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        return pHelpScrollArea;
    }
    return nullptr;
}


QWidget *SystemProperties::createSystemSettings()
{
    QWidget *pSettingsWidget = new QWidget();

    // Load start values or not
    mpKeepValuesAsStartValues = new QCheckBox("Keep values from previous simulation as start values", pSettingsWidget);
    mpKeepValuesAsStartValues->setCheckable(true);
    mpKeepValuesAsStartValues->setChecked(mpSystemObject->getCoreSystemAccessPtr()->doesKeepStartValues());

    // Disable undo check box
    mpEnableUndoCheckBox = new QCheckBox(tr("Enable Undo Function"), pSettingsWidget);
    mpEnableUndoCheckBox->setCheckable(true);
    mpEnableUndoCheckBox->setChecked(mpSystemObject->isUndoEnabled());

    // Save undo check box
    mpSaveUndoCheckBox = new QCheckBox(tr("Save Undo Function In Model File"), pSettingsWidget);
    mpSaveUndoCheckBox->setCheckable(true);
    mpSaveUndoCheckBox->setChecked(mpSystemObject->getSaveUndo());

    // Disalbe animation setting
    mpDisableAnimationCheckBox = new QCheckBox("Disable animation mode for current system (for educational use)", pSettingsWidget);
    mpDisableAnimationCheckBox->setCheckable(true);
    mpDisableAnimationCheckBox->setChecked(mpSystemObject->isAnimationDisabled());

    QGridLayout *pSettingsLayout = new QGridLayout(pSettingsWidget);
    int row = 0;
    pSettingsLayout->addWidget(mpKeepValuesAsStartValues,      row, 0, 1, 2);
    pSettingsLayout->addWidget(mpEnableUndoCheckBox,         ++row, 0, 1, 2);
    pSettingsLayout->addWidget(mpSaveUndoCheckBox,           ++row, 0, 1, 2);
    pSettingsLayout->addWidget(mpDisableAnimationCheckBox,   ++row, 0, 1, 2);
    pSettingsLayout->addWidget(new QWidget(pSettingsWidget), ++row, 0, 1, 2);
    pSettingsLayout->setRowStretch(4, 1);

    // Time step
    mpTimeStepCheckBox = new QCheckBox("Inherit time step from parent system", pSettingsWidget);
    mpTimeStepCheckBox->setChecked(mpSystemObject->getCoreSystemAccessPtr()->doesInheritTimeStep());
    QLabel *pTimeStepLabel = new QLabel(" Time Step:", pSettingsWidget);
    pTimeStepLabel->setDisabled(mpTimeStepCheckBox->isChecked());
    QString timeStepText;
    if(mpTimeStepCheckBox->isChecked())
    {
        timeStepText.setNum(mpSystemObject->getParentSystemObject()->getCoreSystemAccessPtr()->getDesiredTimeStep());
    }
    else
    {
        timeStepText.setNum(mpSystemObject->getCoreSystemAccessPtr()->getDesiredTimeStep());
    }
    mpTimeStepEdit = new QLineEdit(pSettingsWidget);
    mpTimeStepEdit->setValidator(new QDoubleValidator(0.0, 2000000000.0, 10, pSettingsWidget));
    mpTimeStepEdit->setText(timeStepText);
    mpTimeStepEdit->setDisabled(mpTimeStepCheckBox->isChecked());

    connect(mpTimeStepCheckBox, SIGNAL(toggled(bool)), pTimeStepLabel, SLOT(setDisabled(bool)));
    connect(mpTimeStepCheckBox, SIGNAL(toggled(bool)), mpTimeStepEdit, SLOT(setDisabled(bool)));
    connect(mpTimeStepCheckBox, SIGNAL(toggled(bool)), this,           SLOT(fixTimeStepInheritance(bool)));
    pSettingsLayout->addWidget(mpTimeStepCheckBox, row,   0, 1, 2);
    pSettingsLayout->addWidget(pTimeStepLabel,     ++row, 0, 1, 1);
    pSettingsLayout->addWidget(mpTimeStepEdit,     row,   1, 1, 1);

    // Log samples
    mpNumLogSamplesEdit = new QLineEdit(pSettingsWidget);
    mpNumLogSamplesEdit->setValidator(new QIntValidator(0, 2000000000, pSettingsWidget));
    mpNumLogSamplesEdit->setText(QString("%1").arg(mpSystemObject->getNumberOfLogSamples())); //!< @todo what if group
    pSettingsLayout->addWidget(new QLabel(tr("Log Samples:"), pSettingsWidget), ++row,   0);
    pSettingsLayout->addWidget(mpNumLogSamplesEdit,                             row, 1);

    // Log start time
    mpLogStartTimeEdit = new QLineEdit(pSettingsWidget);
    mpLogStartTimeEdit->setValidator(new QDoubleValidator(pSettingsWidget));
    mpLogStartTimeEdit->setText(QString("%1").arg(mpSystemObject->getLogStartTime())); //!< @todo what if group
    pSettingsLayout->addWidget(new QLabel(tr("Log Start:"), pSettingsWidget), ++row, 0);
    pSettingsLayout->addWidget(mpLogStartTimeEdit, row, 1);

    QPushButton *pClearLogDataButton = new QPushButton("Clear All Log Data", pSettingsWidget);
    pClearLogDataButton->setEnabled(!mpSystemObject->getLogDataHandler()->isEmpty());
    connect(pClearLogDataButton, SIGNAL(clicked()), this, SLOT(clearLogData()));
    pSettingsLayout->addWidget(pClearLogDataButton, ++row, 0);

    pSettingsLayout->addWidget(new QWidget(pSettingsWidget), ++row, 0, 1, 2);
    pSettingsLayout->setRowStretch(row, 1);

    return pSettingsWidget;
}

QWidget *SystemProperties::createAppearanceSettings()
{
    QWidget *pAppearanceWidget = new QWidget();

    // Icon paths
    QLabel* pUserIconLabel = new QLabel("User Icon Path:", pAppearanceWidget);
    QLabel* pIsoIconLabel  = new QLabel("ISO Icon Path:",  pAppearanceWidget);
    mpUserIconPath  = new QLineEdit(mpSystemObject->getIconPath(UserGraphics, Absolute), pAppearanceWidget);
    mpIsoIconPath   = new QLineEdit(mpSystemObject->getIconPath(ISOGraphics, Absolute), pAppearanceWidget);
    pUserIconLabel->setMinimumWidth(80);
    mpUserIconPath->setMinimumWidth(200);
    QPushButton* pIsoIconBrowseButton = new QPushButton(tr("..."), pAppearanceWidget);
    QPushButton* pUserIconBrowseButton = new QPushButton(tr("..."), pAppearanceWidget);
    //    mpIsoIconBrowseButton->setFixedSize(25, 22);
    //    mpUserIconBrowseButton->setFixedSize(25, 22);
    pIsoIconBrowseButton->setAutoDefault(false);
    pUserIconBrowseButton->setAutoDefault(false);

    // Icon type
    mpIsoCheckBox = new QCheckBox(tr("Use ISO 1219 Graphics"), pAppearanceWidget);
    mpIsoCheckBox->setCheckable(true);
    mpIsoCheckBox->setChecked(mpSystemObject->getGfxType());

    // Graphics scales
    QLabel *pUserIconScaleLabel = new QLabel("User Icon Scale:", pAppearanceWidget);
    QLabel *pIsoIconScaleLabel = new QLabel("ISO Icon Scale:", pAppearanceWidget);
    mpUserIconScaleEdit = new QLineEdit(pAppearanceWidget);
    mpUserIconScaleEdit->setValidator(new QDoubleValidator(0.1, 10.0, 2, pAppearanceWidget));
    mpUserIconScaleEdit->setText(QString("%1").arg(mpSystemObject->getAppearanceData()->getIconScale(UserGraphics)));
    mpIsoIconScaleEdit = new QLineEdit(pAppearanceWidget);
    mpIsoIconScaleEdit->setValidator(new QDoubleValidator(0.1, 10.0, 2, pAppearanceWidget));
    mpIsoIconScaleEdit->setText(QString("%1").arg(mpSystemObject->getAppearanceData()->getIconScale(ISOGraphics)));


    QGridLayout *pAppearanceLayout = new QGridLayout(pAppearanceWidget);
    int r=0;
    pAppearanceLayout->addWidget(pUserIconLabel, r, 0);
    pAppearanceLayout->addWidget(mpUserIconPath, r, 1);
    pAppearanceLayout->addWidget(pUserIconBrowseButton, r, 2);
    r++;
    pAppearanceLayout->addWidget(pIsoIconLabel, r, 0);
    pAppearanceLayout->addWidget(mpIsoIconPath, r, 1);
    pAppearanceLayout->addWidget(pIsoIconBrowseButton, r, 2);
    r++;
    pAppearanceLayout->addWidget(pUserIconScaleLabel, r, 0);
    pAppearanceLayout->addWidget(mpUserIconScaleEdit, r, 1);
    r++;
    pAppearanceLayout->addWidget(pIsoIconScaleLabel, r, 0);
    pAppearanceLayout->addWidget(mpIsoIconScaleEdit, r, 1);
    r++;
    pAppearanceLayout->addWidget(mpIsoCheckBox, r, 0, 1, -1);
    r++;
    pAppearanceLayout->addWidget(new QWidget(pAppearanceWidget), r, 0, 1, 2);
    pAppearanceLayout->setRowStretch(r, 1);

    connect(pUserIconBrowseButton, SIGNAL(clicked(bool)), this, SLOT(browseUser()));
    connect(pIsoIconBrowseButton, SIGNAL(clicked(bool)), this, SLOT(browseIso()));

    return pAppearanceWidget;
}

QWidget *SystemProperties::createModelinfoSettings()
{
    QString author, email, affiliation, description;
    mpSystemObject->getModelInfo(author, email, affiliation, description);

    QWidget *pInfoWidget = new QWidget();
    QGridLayout *pInfoLayout = new QGridLayout();

    QLabel *pAuthorLabel = new QLabel("Author: ", pInfoWidget);
    QLabel *pEmailLabel = new QLabel("Email: ", pInfoWidget);
    QLabel *pAffiliationLabel = new QLabel("Affiliation: ", pInfoWidget);
    QLabel *pDescriptionLabel = new QLabel("Description: ", pInfoWidget);
    QLabel *pBasePathLabel = new QLabel("BasePath: ", pInfoWidget);

    mpAuthorEdit = new QLineEdit(author, pInfoWidget);
    mpEmailEdit = new QLineEdit(email, pInfoWidget);
    mpAffiliationEdit = new QLineEdit(affiliation, pInfoWidget);
    mpDescriptionEdit = new QTextEdit(description, pInfoWidget);
    mpDescriptionEdit->setFixedHeight(100);
    QLineEdit *pBasePathEdit = new QLineEdit(mpSystemObject->getAppearanceData()->getBasePath(), pInfoWidget);
    pBasePathEdit->setReadOnly(true);

    // If this is a subsystem that is external, then prevent editing model information
    if (mpSystemObject->isExternal() && !mpSystemObject->isTopLevelContainer())
    {
        mpAuthorEdit->setReadOnly(true);
        mpEmailEdit->setReadOnly(true);
        mpAffiliationEdit->setReadOnly(true);
        mpDescriptionEdit->setReadOnly(true);
    }

    pInfoLayout->addWidget(pAuthorLabel,       0, 0);
    pInfoLayout->addWidget(mpAuthorEdit,       0, 1);
    pInfoLayout->addWidget(pEmailLabel,        1, 0);
    pInfoLayout->addWidget(mpEmailEdit,        1, 1);
    pInfoLayout->addWidget(pAffiliationLabel,  2, 0);
    pInfoLayout->addWidget(mpAffiliationEdit,  2, 1);
    pInfoLayout->addWidget(pDescriptionLabel,  3, 0);
    pInfoLayout->addWidget(mpDescriptionEdit,  3, 1);
    pInfoLayout->addWidget(pBasePathLabel,     4, 0);
    pInfoLayout->addWidget(pBasePathEdit,      4, 1);
    pInfoLayout->addWidget(new QWidget(pInfoWidget), 5, 0, 1, 2);
    pInfoLayout->setRowStretch(5, 1);
    pInfoWidget->setLayout(pInfoLayout);

    return pInfoWidget;
}

void ComponentPropertiesDialog3::createEditStuff()
{
    QGridLayout* pPropertiesLayout = new QGridLayout(this);
    int row=0;

    // Parents to new objects bellow should be set automatically when adding layout or widget to other layout or widget

    // Add help picture and text
    //------------------------------------------------------------------------------------------------------------------------------
    QWidget *pHelpWidget = createHelpWidget();
    //------------------------------------------------------------------------------------------------------------------------------

    // Add name edit and type information
    //------------------------------------------------------------------------------------------------------------------------------
    QGridLayout *pNameTypeLayout = createNameAndTypeEdit();
    pPropertiesLayout->addLayout(pNameTypeLayout, row, 0, Qt::AlignLeft);
    pPropertiesLayout->setRowStretch(row,0);
    pNameTypeLayout->setEnabled(mAllowEditing);
    //------------------------------------------------------------------------------------------------------------------------------

    // Add button widget with description, move ports and copy to clipboard buttons
    //------------------------------------------------------------------------------------------------------------------------------
    QWidget *pDCEButtonWidget = new QWidget(this);
    QVBoxLayout *pDCButtonsLayout = new QVBoxLayout(pDCEButtonWidget);
    QPushButton *pDescriptionButton = new QPushButton(tr("&Description"), this);
    pDescriptionButton->setToolTip("Open description in separate window");
    QPushButton *pEditPortsButton = new QPushButton(tr("&Move ports"), this);
    pEditPortsButton->setEnabled(mAllowEditing);
    connect(pDescriptionButton, SIGNAL(clicked()), this, SLOT(openDescription()));
    connect(pEditPortsButton, SIGNAL(clicked()), this, SLOT(editPortPos()));
    pDCButtonsLayout->addWidget(pDescriptionButton);
    pDCButtonsLayout->addWidget(pEditPortsButton);
    pPropertiesLayout->addWidget(pDCEButtonWidget, row, 1);
    //------------------------------------------------------------------------------------------------------------------------------
    ++row;

    // Add list with parameter sets (if provided by component)
    //------------------------------------------------------------------------------------------------------------------------------
    QMap<QString, QString> sets = mpModelObject->getParameterSets();
    if(!sets.isEmpty()) {
        qDebug() << "Adding parameter sets to dialog";
        QHBoxLayout *pParameterSetsLayout = new QHBoxLayout();
        QLabel *pSetsLabel = new QLabel("Parameter Sets:",this);
        mpSetsComboBox = new QComboBox(this);
        mpSetsComboBox->addItems(sets.keys());
        QPushButton *pApplySetButton = new QPushButton("Apply");
        connect(pApplySetButton, SIGNAL(clicked(bool)), this, SLOT(applyParameterSet()));
        pParameterSetsLayout->addWidget(pSetsLabel);
        pParameterSetsLayout->addWidget(mpSetsComboBox);
        pParameterSetsLayout->addWidget(pApplySetButton);
        pParameterSetsLayout->addWidget(new QWidget(this));
        pParameterSetsLayout->setStretch(3,1);
        pPropertiesLayout->addLayout(pParameterSetsLayout, row, 0, 1, 1);
    }
    //------------------------------------------------------------------------------------------------------------------------------
    ++row;

    // Add Parameter settings table
    //------------------------------------------------------------------------------------------------------------------------------
    mpVariableTableWidget = new VariableTableWidget(mpModelObject,this);
    mpVariableTableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    pPropertiesLayout->addWidget(mpVariableTableWidget, row, 0, 1, 3);
    pPropertiesLayout->setRowStretch(row,1);
    mpVariableTableWidget->setEnabled(mAllowEditing);
    qDebug() << "Table: " << mpVariableTableWidget->sizeHint() << "  " << mpVariableTableWidget->minimumWidth() << "  " << mpVariableTableWidget->minimumHeight();
    //------------------------------------------------------------------------------------------------------------------------------

    QWidget *pPropertiesWidget = new QWidget(this);
    pPropertiesWidget->setLayout(pPropertiesLayout);
    pPropertiesWidget->setPalette(gpConfig->getPalette());

    // Add Code edit stuff, A new tab in a new widget will be created
    //------------------------------------------------------------------------------------------------------------------------------

    QGridLayout *pMainLayout = new QGridLayout(this);

    QTabWidget *pTabWidget = new QTabWidget(this);
    pTabWidget->addTab(pPropertiesWidget, "Properties");
    pTabWidget->addTab(pHelpWidget, "Description");

    // Add tabs for subsystems
    if (mpModelObject->type() == SystemObjectType)
    {
        mpSystemProperties = new SystemProperties(qobject_cast<SystemObject*>(mpModelObject), this);
        QWidget *pSystemSettingsWidget = mpSystemProperties->createSystemSettings();
        QWidget *pSystemAppearanceWidget = mpSystemProperties->createAppearanceSettings();
        QWidget *pModelInfoWidget = mpSystemProperties->createModelinfoSettings();
        pTabWidget->addTab(pSystemSettingsWidget, "SystemSettings");
        pTabWidget->addTab(pSystemAppearanceWidget, "Appearance");
        pTabWidget->addTab(pModelInfoWidget, "ModelInfo");
    }

    pMainLayout->addWidget(pTabWidget);

    QDialogButtonBox *pOKButtonBox = createOKButtonBox();
    pMainLayout->addWidget(pOKButtonBox);

    //------------------------------------------------------------------------------------------------------------------------------

    setLayout(pMainLayout);

    // Avoid a dialog that is higher than the available space
    int maxHeight = qApp->desktop()->screenGeometry().height()-50;
    this->setMaximumHeight(maxHeight);
}

VariableTableWidget::VariableTableWidget(ModelObject *pModelObject, QWidget *pParent) :
    TableWidgetTotalSize(pParent)
{
    mpModelObject = pModelObject;

    this->setColumnCount(NumCols);
    QStringList columnHeaders;
    columnHeaders.append("Name");
    columnHeaders.append("Alias");
    columnHeaders.append("Description");
    columnHeaders.append("Unit");
    columnHeaders.append("Value");
    columnHeaders.append("Quantity");
    columnHeaders.append("PlotLabel");
    columnHeaders.append("Port");
    this->setHorizontalHeaderLabels(columnHeaders);

    // Decide which parameters should be shown as Constants
    QVector<CoreParameterData> parameters;
    mpModelObject->getParameters(parameters);
    QVector<int> constantsIds;
    for (int i=0; i<parameters.size(); ++i)
    {
        if (!parameters[i].mName.contains("#"))
        {
            constantsIds.push_back(i);
        }
    }

    // Write the constants first
    int r=0;
    if (!constantsIds.isEmpty())
    {
        createSeparatorRow(r,"Constants", Constant);
        ++r;
    }
    for (int i=0; i<constantsIds.size(); ++i)
    {
        CoreVariameterDescription variameter;
        variameter.mName = parameters[constantsIds[i]].mName;

        if(mpModelObject->getAppearanceData()->isParameterHidden(variameter.mName))
            continue;

        variameter.mDescription = parameters[constantsIds[i]].mDescription;
        variameter.mUnit = parameters[constantsIds[i]].mUnit;
        variameter.mQuantity = parameters[constantsIds[i]].mQuantity;
        variameter.mDataType = parameters[constantsIds[i]].mType;
        variameter.mConditions = parameters[constantsIds[i]].mConditions;
        createTableRow(r, variameter, Constant);
        ++r;
    }

    // Now fetch and write the input variables
    QVector<CoreVariameterDescription> variameters;
    mpModelObject->getVariameterDescriptions(variameters);

    // Write inputVariables
    const int inputVarSeparatorRow = r;
    for (int i=0; i<variameters.size(); ++i)
    {
        if(mpModelObject->getAppearanceData()->isParameterHidden(variameters[i].mPortName+"#"+variameters[i].mName))
            continue;

        if (variameters[i].mVariameterType == InputVaraiable)
        {
            createTableRow(r, variameters[i], InputVaraiable);
            ++r;
        }
    }
    // Insert the separator row if there were any input variables)
    if (r != inputVarSeparatorRow)
    {
        createSeparatorRow(inputVarSeparatorRow,"InputVariables", InputVaraiable);
        ++r;
    }

    // Write outputVariables
    const int outputVarSeparatorRow = r;
    for (int i=0; i<variameters.size(); ++i)
    {
        if (variameters[i].mVariameterType == OutputVariable)
        {
            createTableRow(r, variameters[i], OutputVariable);
            ++r;
        }
    }
    // Insert the separator row if there were any output variables)
    if (r != outputVarSeparatorRow)
    {
        createSeparatorRow(outputVarSeparatorRow,"OutputVariables", OutputVariable);
        ++r;
    }

    // Write remaining port variables
    QString currPortName;
    for (int i=0; i<variameters.size(); ++i)
    {
        if ( (variameters[i].mVariameterType == OtherVariable) &&
            ( gpConfig->getBoolSetting(cfg::showhiddennodedatavariables) || (variameters[i].mVariabelType != "Hidden") ) )
        {
            // Extract current port name to see if we should make a separator
            QString portName = variameters[i].mPortName;
            if (portName != currPortName)
            {
                currPortName = portName;
                Port* pPort = mpModelObject->getPort(portName);
                if (pPort)
                {
                    QString desc = pPort->getPortDescription();
                    if (!desc.isEmpty())
                    {
                        portName.append("     "+desc);
                    }
                }
                createSeparatorRow(r,"Port: "+portName, OtherVariable);
                ++r;
            }

            createTableRow(r, variameters[i], OtherVariable);
            ++r;
        }
    }

    resizeColumnToContents(Name);
    resizeColumnToContents(Unit);
    resizeColumnToContents(Value);
    resizeColumnToContents(Quantity);
    resizeColumnToContents(PlotSettings);
    resizeColumnToContents(ShowPort);
    setColumnWidth(Description, 2*columnWidth(Description));

#if QT_VERSION >= 0x050000
    horizontalHeader()->setSectionResizeMode(Name, QHeaderView::ResizeToContents);
//    horizontalHeader()->setSectionResizeMode(Alias, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(Description, QHeaderView::Stretch);
    horizontalHeader()->setSectionResizeMode(Value, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(Unit, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(Quantity, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(PlotSettings, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(ShowPort, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionsClickable(false);
#else
    horizontalHeader()->setResizeMode(Name, QHeaderView::ResizeToContents);
//    horizontalHeader()->setResizeMode(Alias, QHeaderView::ResizeToContents);
    horizontalHeader()->setResizeMode(Description, QHeaderView::Stretch);
    horizontalHeader()->setResizeMode(Value, QHeaderView::ResizeToContents);
    horizontalHeader()->setResizeMode(Unit, QHeaderView::ResizeToContents);
    horizontalHeader()->setResizeMode(Quantity, QHeaderView::ResizeToContents);
    horizontalHeader()->setResizeMode(PlotSettings, QHeaderView::ResizeToContents);
    horizontalHeader()->setResizeMode(ShowPort, QHeaderView::ResizeToContents);
    horizontalHeader()->setClickable(false);
#endif

    verticalHeader()->hide();
}

bool VariableTableWidget::setStartValues()
{
    //Hack that will make sure all values currently being edited is set before using them
    setDisabled(true);
    setEnabled(true);

    bool addedUndoPost = false;
    bool allok=true;
    for (int row=0; row<rowCount(); ++row)
    {

        // First check if row is separator, then skip it
        if (columnSpan(row,0)>1)
        {
            continue;
        }

        // If startvalue is empty (disabled, then we should not attempt to change it)
        bool isDisabled = qobject_cast<ParameterValueSelectionWidget*>(cellWidget(row, int(VariableTableWidget::Value)))->isValueDisabled();
        if (isDisabled)
        {
            continue;
        }

        ParameterValueSelectionWidget *pValueWideget = qobject_cast<ParameterValueSelectionWidget*>(cellWidget(row, int(VariableTableWidget::Value)));
        // Extract name and value from row
        QString name = pValueWideget->getName();
        QString value = pValueWideget->getValueText();

        // Check if we have new custom scaling
        UnitConverter newCustomUnitScale, previousUnitScale;
        mpModelObject->getCustomParameterUnitScale(name, previousUnitScale);
        UnitSelectionWidget *pUnitWidget = pValueWideget->getUnitSelectionWidget();
        if (pUnitWidget)
        {
            // Check if this is a numeric value
            bool isDouble=false;
            value.toDouble(&isDouble);

            // Unregister unit scale if default unit has been reset or if the value is not a number (but a parameter name or expression, (Actually a string))
            if (pUnitWidget->isDefaultSelected() || !isDouble)
            {
                mpModelObject->unregisterCustomParameterUnitScale(name);
            }
            // Else register (remember) the new unit scale
            else
            {
                pUnitWidget->getSelectedUnitScale(newCustomUnitScale);
                mpModelObject->registerCustomParameterUnitScale(name, newCustomUnitScale);
            }
        }

        // If we have a custom unit scale then undo the scale and set a value expressed in the default unit
        if (!newCustomUnitScale.isEmpty())
        {
            value = newCustomUnitScale.convertToBase(value);
        }

        // Get the old value to see if a change has occurred
        // We also check the unit scale as that may have changed to even if the value (in original unit) is the same
        QString previousValue = mpModelObject->getParameterValue(name);
        bool setNewValueSucess=false;
        if ((previousValue != value) || (previousUnitScale != newCustomUnitScale))
        {
            // Parameter has changed, add to undo stack and set the parameter
            if( cleanAndVerifyParameterValue(value, qobject_cast<ParameterValueSelectionWidget*>(cellWidget(row, int(VariableTableWidget::Value)))->getDataType()) )
            {
                if(!mpModelObject->hasParameter(name)) {
                    continue;
                }
                // Try to set the actual parameter
                if(mpModelObject->setParameterValue(name, value))
                {
                    // Add an undo post (but only one for all values changed this time
                    if(!addedUndoPost)
                    {
                        mpModelObject->getParentSystemObject()->getUndoStackPtr()->newPost(undo::changedparameters);
                        addedUndoPost = true;
                    }
                    // Register the change in undo stack
                    mpModelObject->getParentSystemObject()->getUndoStackPtr()->registerChangedParameter(mpModelObject->getName(),
                                                                                                           name,
                                                                                                           previousValue,
                                                                                                           value);
                    // Mark project tab as changed
                    mpModelObject->getParentSystemObject()->hasChanged();

                    setNewValueSucess =true;
                }
                // If we fail to set the parameter, then warning box
                else
                {
                    QMessageBox::critical(nullptr, "Hopsan GUI", QString("'%1' is an invalid value for parameter '%2'. Resetting old value '%3'!").arg(value).arg(name).arg(previousValue));
                }
            }

            // Reset old value
            if (!setNewValueSucess)
            {
                ParameterValueSelectionWidget *pWidget = qobject_cast<ParameterValueSelectionWidget*>(cellWidget(row, int(VariableTableWidget::Value)));
                if (isNumber(previousValue))
                {
                    pWidget->setValueAndScale_nosignals(previousUnitScale.convertFromBase(previousValue), previousUnitScale);
                    mpModelObject->registerCustomParameterUnitScale(name, previousUnitScale);
                }
                else
                {
                    pWidget->setValueAndScale_nosignals(previousValue, previousUnitScale);
                }
                allok = false;
                break;
            }
        }
        //! @todo if we set som ok but som later fail pressing cancel will not restore the first that were set ok /Peter
    }
    return allok;
}

bool VariableTableWidget::setAliasNames()
{
    for (int r=0; r<rowCount(); ++r)
    {
        if (!setAliasName(r))
        {
            return false;
        }
    }
    return true;
}


//! @brief Sets the value in the parameter value line edit
//!
//! Actual parameter value is not affected until "Apply" or "Ok" buttons are presed.
//!
//! @param rName[in] Name of parameter
//! @param rValue[in] New value for parameter
void VariableTableWidget::setValue(const QString &rName, const QString &rValue)
{
    for(int row=0; row<rowCount(); ++row) {
        ParameterValueSelectionWidget *pValueWidget = qobject_cast<ParameterValueSelectionWidget*>(cellWidget(row, int(VariableTableWidget::Value)));
        if(pValueWidget != nullptr && pValueWidget->getName() == rName) {
            pValueWidget->setValueText(rValue);
            pValueWidget->refreshValueTextStyle();
        }
    }
}


//! @brief Returns currently selected unit for specified parameter
//! @param[in] rName Name of parameter
//! @returns Currently selected unit in drop-down menu
QString VariableTableWidget::getSelectedUnit(const QString &rName)
{
    for(int row=0; row<rowCount(); ++row) {
        ParameterValueSelectionWidget *pValueWidget = qobject_cast<ParameterValueSelectionWidget*>(cellWidget(row, int(VariableTableWidget::Value)));
        if(pValueWidget != nullptr && pValueWidget->getName() == rName) {
            return pValueWidget->getUnitSelectionWidget()->getSelectedUnit();
        }
    }
    return QString();   //Parameter not found, return empty string
}

bool VariableTableWidget::focusNextPrevChild(bool next)
{
    QWidget* pPrevWidget = indexWidget(currentIndex());
    QTableWidgetItem* pPrevItem=currentItem();
    if (focusWidget() == this)
    {
        if (pPrevWidget)
        {
            pPrevWidget->setFocus();
        }
    }

    bool retval = true;
    QWidget* pCurrentWidget = pPrevWidget;
    QTableWidgetItem* pNewItem=pPrevItem;
    int ctr=0;
    while (retval && (pPrevWidget==pCurrentWidget) && (pPrevItem==pNewItem) )
    {
        retval = TableWidgetTotalSize::focusNextPrevChild(next);
        pCurrentWidget = indexWidget(currentIndex());
        pNewItem = currentItem();
        ctr++;
        // Sometimes the focusNextPrevChild returns true but our table index has not changed
        // if that happens 10 times in a row we abort with failure, else we would get stuck in endless loops/recursion in this function
        if (ctr > 10)
        {
            retval = false;
            break;
        }
    }

    int col = currentColumn();
    int row = currentRow();

    //Skip non-editable columns and separator rows
    if(columnSpan(row,0)>2 && next && row != rowCount()-1)
    {
        ++row;
        setCurrentCell(row,col);
    }
    else if(columnSpan(row,0)>2 && !next && row != 0)
    {
        --row;
        setCurrentCell(row,col);
    }

    // Refresh if we changed rows
    col = currentColumn();
    row = currentRow();
    pCurrentWidget = indexWidget(currentIndex());

    if(row == 0)
    {
        return false;
    }
    else if(col == Value)
    {
        ParameterValueSelectionWidget *pParWidget = qobject_cast<ParameterValueSelectionWidget*>(pCurrentWidget);
        if(pParWidget && pParWidget->getValueEditPtr())
        {
            pParWidget->getValueEditPtr()->setFocus();
        }
    }
    //! @todo quantity input?
//    else if(currentColumn() == Quantity)
//    {
//        PlotScaleSelectionWidget *pScaleWidget = qobject_cast<PlotScaleSelectionWidget*>(pIndexWidget);
//        pScaleWidget->getPlotScaleEditPtr()->setFocus();
//    }
    else if(col == PlotSettings)
    {
        PlotSettingsWidget *pPlotSettingsWidget = qobject_cast<PlotSettingsWidget*>(pCurrentWidget);
        if(pPlotSettingsWidget && pPlotSettingsWidget->plotLabel())
        {
            pPlotSettingsWidget->plotLabel()->setFocus();
        }
    }
    else if(col == ShowPort)
    {
        HideShowPortWidget *pPortWidget = qobject_cast<HideShowPortWidget*>(pCurrentWidget);
        if(pPortWidget && pPortWidget->getCheckBoxPtr())
        {
            pPortWidget->getCheckBoxPtr()->setFocus();
        }
    }
    // If we are at any of these columns we should skip ahead (focusNext or focusPrev), since they do not support input
    else if (col == Name || col == Description || col == Unit || col == Quantity || col == NumCols)
    {
        while(retval && (col == Name || col == Description || col == Unit || col == Quantity || col == NumCols))
        {
            retval = focusNextPrevChild(next);
            col = currentColumn();
            row = currentRow();
        }
    }
    // This will trigger for all other colums (ordinary table items)
    else
    {
        this->setFocus();
    }

    return retval;
}

void VariableTableWidget::createTableRow(const int row, const CoreVariameterDescription &rData, const VariameterTypEnumT variametertype)
{
    QString fullName, name;
    QTableWidgetItem *pItem;
    insertRow(row);

    //! @todo maybe store the variameter data objects locally, and check for hidden info there, would also make it possible to check for changes without asking core all of the time /Peter

    if (variametertype == Constant)
    {
        fullName = rData.mName;
        name = rData.mName;
    }
    else if (variametertype == OtherVariable)
    {
        if (rData.mPortName.isEmpty())
        {
            fullName = rData.mName;
        }
        else
        {
            fullName = rData.mPortName+"#"+rData.mName;
        }
        name = rData.mName;
    }
    else //For input and output variables
    {
        fullName = rData.mPortName+"#"+rData.mName;
        name = rData.mPortName;
    }

    // Set the name field
    pItem = new QTableWidgetItem(name);
    pItem->setToolTip(fullName);
    pItem->setTextAlignment(Qt::AlignCenter);
    pItem->setFlags(Qt::NoItemFlags | Qt::ItemIsEnabled);
    setItem(row,Name,pItem);

    // Set the alias name field
    pItem = new QTableWidgetItem(rData.mAlias);
    pItem->setTextAlignment(Qt::AlignCenter);
    pItem->setFlags(Qt::NoItemFlags);
    if (variametertype != Constant)
    {
        pItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);
    }
    setItem(row,Alias,pItem);

    // Set the description field
    pItem = new QTableWidgetItem(rData.mDescription);
    pItem->setTextAlignment(Qt::AlignCenter);
    pItem->setFlags(Qt::NoItemFlags | Qt::ItemIsEnabled);
    pItem->setToolTip(rData.mDescription);
    setItem(row,Description,pItem);

    // Set the unit field
    pItem = new QTableWidgetItem(rData.mUnit);
    pItem->setTextAlignment(Qt::AlignCenter);
    pItem->setFlags(Qt::NoItemFlags | Qt::ItemIsEnabled);
    setItem(row,Unit,pItem);

    // Set the value field
    ParameterValueSelectionWidget *pValueWidget = new ParameterValueSelectionWidget(rData, variametertype, mpModelObject, this);
    this->setIndexWidget(model()->index(row,Value), pValueWidget);
    // Trigger signal to unit selector if syspar entered to disable the unit scroll box
    pValueWidget->checkIfSysParEntered();

    // Create the quantity widget
    if (variametertype != Constant)
    {
//        QWidget *pPlotScaleWidget = new PlotScaleSelectionWidget(rData, mpModelObject, this);
//        this->setIndexWidget(model()->index(row,Scale), pPlotScaleWidget);
        QWidget *pQuantityWidget = new QuantitySelectionWidget(rData, mpModelObject, this);
        setIndexWidget(model()->index(row,Quantity), pQuantityWidget);
    }
    else
    {
        pItem = new QTableWidgetItem();
        pItem->setFlags(Qt::NoItemFlags);
        setItem(row,Quantity,pItem);
    }

    // Set invert plot check box
    if (variametertype != Constant)
    {
        QWidget *pPlotWidget = new PlotSettingsWidget(rData, mpModelObject, this);
        setIndexWidget(model()->index(row,PlotSettings), pPlotWidget);
    }
    else
    {
        pItem = new QTableWidgetItem();
        pItem->setFlags(Qt::NoItemFlags);
        setItem(row,PlotSettings,pItem);
    }


    // Set the port hide/show button
    if ( (variametertype == InputVaraiable) || (variametertype == OutputVariable))
    {
        auto *pWidget = new HideShowPortWidget(rData, mpModelObject, this);
        connect(pWidget, SIGNAL(toggled(bool)), pValueWidget, SLOT(refreshValueTextStyle()));
        connect(this, &VariableTableWidget::refreshInternalPortInfo, pWidget, &HideShowPortWidget::refreshPortToggleState);
        this->setIndexWidget(model()->index(row,ShowPort), pWidget);
    }
    else
    {
        pItem = new QTableWidgetItem();
        pItem->setFlags(Qt::NoItemFlags);
        setItem(row,ShowPort,pItem);
    }
}

void VariableTableWidget::createSeparatorRow(const int row, const QString &rName, const VariameterTypEnumT variametertype)
{
    insertRow(row);

    QTableWidgetItem *pItem, *pItem2=nullptr;
    pItem = new QTableWidgetItem(rName);
    pItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    pItem->setFlags(Qt::NoItemFlags | Qt::ItemIsEnabled);
    pItem->setBackgroundColor(Qt::lightGray);

    if ( variametertype == InputVaraiable )
    {
        pItem2 = new QTableWidgetItem("Default Value");
    }
    else if ( variametertype == OutputVariable )
    {
        pItem2 = new QTableWidgetItem("Start Value");
    }
    else if ( variametertype == OtherVariable )
    {
        pItem2 = new QTableWidgetItem("Start Value");
    }

    if (pItem2)
    {
        pItem2->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        pItem2->setFlags(Qt::NoItemFlags | Qt::ItemIsEnabled);
        pItem2->setBackgroundColor(Qt::lightGray);

        setSpan(row,Name,1,Value);
        setItem(row,Name,pItem);
        setSpan(row,Value,1,NumCols-Value);
        setItem(row,Value,pItem2);
    }
    else
    {
        setSpan(row,Name,1,NumCols);
        setItem(row,Name,pItem);
    }
    resizeRowToContents(row);
}

TableWidgetTotalSize::TableWidgetTotalSize(QWidget *pParent) : QTableWidget(pParent)
{
    mMaxVisibleRows=20;
}

QSize TableWidgetTotalSize::sizeHint() const
{
    int w=0;
    if (verticalHeader()->isVisible())
    {
        w += verticalHeader()->sizeHint().width();
    }
    w +=  + frameWidth()*2 + verticalScrollBar()->sizeHint().width();
    //qDebug() << "w: " << w << " lw: " << lineWidth() << "  mLw: " << midLineWidth() << "  frameWidth: " << frameWidth();
    //qDebug() << verticalScrollBar()->sizeHint().width();

    for (int c=0; c<columnCount(); ++c)
    {
        w += columnWidth(c);
    }

    int h = horizontalHeader()->sizeHint().height() + frameWidth()*2;
    for (int r=0; r<qMin(mMaxVisibleRows,rowCount()); ++r)
    {
        h += rowHeight(r);
    }
    return QSize(w, h);
}

void TableWidgetTotalSize::setMaxVisibleRows(const int maxRows)
{
    mMaxVisibleRows = maxRows;
}


ParameterValueSelectionWidget::ParameterValueSelectionWidget(const CoreVariameterDescription &rData, VariableTableWidget::VariameterTypEnumT type, ModelObject *pModelObject, QWidget *pParent) :
    QWidget(pParent)
{
    mpValueEdit = nullptr;
    mpConditionalValueComboBox = nullptr;
    mpUnitSelectionWidget = nullptr;
    mDefaultUnitScale.setOnlyScaleAndOffset(1);
    mpModelObject = pModelObject;
    mVariameterType = type;
    mVariablePortName = rData.mPortName;

    //! @todo maybe store the variameter data objects locally, and check for hidden info there, would also make it possible to check for changes without asking core all of the time /Peter
    if (rData.mPortName.isEmpty())
    {
        mVariablePortDataName = rData.mName;
    }
    else
    {
        mVariablePortDataName = rData.mPortName+"#"+rData.mName;
    }
    mVariableDataType = rData.mDataType;

    QHBoxLayout* pLayout = new QHBoxLayout(this);
    QMargins margins = pLayout->contentsMargins(); margins.setBottom(0); margins.setTop(0);
    pLayout->setContentsMargins(margins);


    if (!rData.mDataType.isEmpty())
    {
        QLabel *pTypeLetter = new QLabel(rData.mDataType[0], this);
        pTypeLetter->setToolTip("DataType: "+rData.mDataType);
        pLayout->addWidget(pTypeLetter);
    }

    // Only set the rest if a value exist (it does not for disabled startvalues)
    if (mpModelObject->hasParameter(mVariablePortDataName))
    {
        QString value = mpModelObject->getParameterValue(mVariablePortDataName);

        mpValueEdit = new ValueEdit(this);
        mpValueEdit->setAlignment(Qt::AlignCenter);
        mpValueEdit->setFrame(false);

        if(mVariableDataType == "conditional")
        {
            mpConditionalValueComboBox = new QComboBox(this);
            for(int i=0; i<rData.mConditions.size(); ++i)
            {
                mpConditionalValueComboBox->addItem(rData.mConditions[i]);
            }
            mpConditionalValueComboBox->setCurrentIndex(value.toInt());
            mpValueEdit->setText(value);
            pLayout->addWidget(mpConditionalValueComboBox);
            connect(mpConditionalValueComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setConditionalValue(int)));
            mpValueEdit->hide();
        }
        else
        {
            pLayout->addWidget(mpValueEdit);
            UnitConverter currentCustomUS;
            if (mpModelObject->getCustomParameterUnitScale(mVariablePortDataName, currentCustomUS))
            {
                mCustomScale = currentCustomUS;
                mpValueEdit->setText(mCustomScale.convertFromBase(value));
            }
            else
            {
                mpValueEdit->setText(value);
            }
            mpValueEdit->setToolTip(mpValueEdit->text());
            connect(mpValueEdit, SIGNAL(editingFinished()), this, SLOT(setValue()));
            connect(mpValueEdit, SIGNAL(textChanged(QString)), this, SLOT(checkIfSysParEntered()));
            connect(mpValueEdit, SIGNAL(textChanged(QString)), mpValueEdit, SLOT(updateToolTip(QString)));
            refreshValueTextStyle();

            mDefaultUnitScale.setOnlyScaleAndOffset(1);

            // Set the unit selection field based on Quantity
            QString quantity = rData.mQuantity;
            // If quantity is empty, first try to figure out from unit (assuming base units)
            if (quantity.isEmpty())
            {
                if (gpConfig->isRegisteredBaseUnit(rData.mUnit))
                {
                    QStringList pqs = gpConfig->getQuantitiesForUnit(rData.mUnit);
                    // Only allow listing in case we have one unique quantity match for base unit
                    if (pqs.size() == 1)
                    {
                        quantity = pqs.front();
                    }
                }
            }

            // Get the default unit scale (should be one but also contains quantity and unit name)
            if (!quantity.isEmpty())
            {
                UnitConverter us;
                gpConfig->getUnitScale(quantity, rData.mUnit, us);
                if (!us.isEmpty())
                {
                    mDefaultUnitScale = us;
                }
            }


            if (!quantity.isEmpty())
            {
                mpUnitSelectionWidget = new UnitSelectionWidget(quantity, this);
                mpUnitSelectionWidget->setUnitScaling(currentCustomUS);
                connect(mpUnitSelectionWidget, SIGNAL(unitChanged(UnitConverter)), this, SLOT(rescaleByUnitScale(UnitConverter)));
                pLayout->addWidget(mpUnitSelectionWidget);

            }
        }

        if (rData.mDataType == "textblock") {
            QToolButton *pEditButton =  new QToolButton(this);
            pEditButton->setIcon(QIcon(QString(ICONPATH) + "svg/Hopsan-Script.svg"));
            pEditButton->setToolTip("Edit");
            pEditButton->setFixedSize(24,24);
            connect(pEditButton, SIGNAL(clicked()), this, SLOT(openValueEditDialog()));
            pLayout->addWidget(pEditButton);
            mpValueEdit->setReadOnly(true);
        }

        if (rData.mDataType == "filepath") {
            QToolButton *pBrowseButton =  new QToolButton(this);
            pBrowseButton->setIcon(QIcon(QString(ICONPATH) + "svg/Hopsan-Open.svg"));
            pBrowseButton->setToolTip("Browse");
            pBrowseButton->setFixedSize(24,24);
            connect(pBrowseButton, SIGNAL(clicked()), this, SLOT(openFileBrowserDialog()));
            pLayout->addWidget(pBrowseButton);
        }

        QToolButton *pResetButton =  new QToolButton(this);
        pResetButton->setIcon(QIcon(QString(ICONPATH) + "svg/Hopsan-ResetDefault.svg"));
        pResetButton->setToolTip("Reset Default Value");
        pResetButton->setFixedSize(24,24);
        connect(pResetButton, SIGNAL(clicked()), this, SLOT(resetDefault()));
        pLayout->addWidget(pResetButton);

        if(mVariableDataType != "conditional")
        {
            QToolButton *pSystemParameterToolButton = new QToolButton(this);
            pSystemParameterToolButton->setIcon(QIcon(QString(ICONPATH) + "svg/Hopsan-SystemParameters.svg"));
            pSystemParameterToolButton->setToolTip("Map To System Parameter");
            pSystemParameterToolButton->setFixedSize(24,24);
            connect(pSystemParameterToolButton, SIGNAL(clicked()), this, SLOT(createSysParameterSelectionMenu()));
            pLayout->addWidget(pSystemParameterToolButton);
        }
    }
    else
    {
        QLabel *pLabel = new QLabel("Disabled",this);
        pLabel->setStyleSheet("color: Gray;");
        pLayout->addWidget(pLabel, 1, Qt::AlignCenter);
    }
}

void ParameterValueSelectionWidget::setValueText(const QString &rText)
{
    if (mpValueEdit)
    {
        mpValueEdit->setText(rText);
        if (mpConditionalValueComboBox)
        {
            mpConditionalValueComboBox->setCurrentIndex(rText.toInt());
        }
    }
}

QString ParameterValueSelectionWidget::getValueText() const
{
    if (mpValueEdit)
    {
        return mpValueEdit->text();
    }
    else
    {
        return QString();
    }
}

const QString &ParameterValueSelectionWidget::getDataType() const
{
    return mVariableDataType;
}

const QString &ParameterValueSelectionWidget::getName() const
{
    return mVariablePortDataName;
}

UnitSelectionWidget *ParameterValueSelectionWidget::getUnitSelectionWidget()
{
    return mpUnitSelectionWidget;
}

bool ParameterValueSelectionWidget::isValueDisabled() const
{
    return (mpValueEdit == nullptr);
}

QLineEdit *ParameterValueSelectionWidget::getValueEditPtr() const
{
    return mpValueEdit;
}

//! @brief Use this to reset values without triggering rescale signals and such things
void ParameterValueSelectionWidget::setValueAndScale_nosignals(QString value, UnitConverter &rCustomUS)
{
    mpValueEdit->blockSignals(true);

    mCustomScale=rCustomUS;
    mpValueEdit->setText(value);

    if (mpUnitSelectionWidget)
    {
        mpUnitSelectionWidget->blockSignals(true);
        if (mCustomScale.isEmpty())
        {
            mpUnitSelectionWidget->resetDefault();
        }
        else
        {
            mpUnitSelectionWidget->setUnitScaling(mCustomScale);
        }
        mpUnitSelectionWidget->blockSignals(false);
    }

    mpValueEdit->blockSignals(false);
}

void ParameterValueSelectionWidget::setValue()
{
    refreshValueTextStyle();
    if (mpValueEdit && mpUnitSelectionWidget)
    {
        bool isNumeric;
        mpValueEdit->text().toDouble(&isNumeric);
        if (!isNumeric)
        {
            mpUnitSelectionWidget->resetDefault();
        }
    }
}

void ParameterValueSelectionWidget::setConditionalValue(const int idx)
{
    if (mpValueEdit)
    {
        mpValueEdit->setText(QString("%1").arg(idx));
    }
}

void ParameterValueSelectionWidget::resetDefault()
{
    // It is important to reset the unit before resetting the value
    // it will make any custom unit scale to be reset first
    if (mpUnitSelectionWidget)
    {
        mpUnitSelectionWidget->resetDefault();
    }

    if(mpModelObject && mpValueEdit)
    {
        QString defaultText = mpModelObject->getDefaultParameterValue(mVariablePortDataName);
        if(!defaultText.isEmpty() || mVariableDataType=="string" || mVariableDataType=="textblock" || mVariableDataType=="filepath")
        {
            mpValueEdit->setText(defaultText);
            setDefaultValueTextStyle();

            if (mpConditionalValueComboBox)
            {
                mpConditionalValueComboBox->setCurrentIndex(defaultText.toInt());
            }
        }
    }
}

void ParameterValueSelectionWidget::createSysParameterSelectionMenu()
{
    QMenu menu;
    QMap<QAction*, QString> actionParamMap;

    QVector<CoreParameterData> paramDataVector;
    mpModelObject->getParentSystemObject()->getParameters(paramDataVector);

    for (int i=0; i<paramDataVector.size(); ++i)
    {
        QAction *tempAction = menu.addAction(paramDataVector[i].mName+" = "+paramDataVector[i].mValue);
        tempAction->setIconVisibleInMenu(false);
        actionParamMap.insert(tempAction, paramDataVector[i].mName);
    }

    if(!menu.isEmpty())
    {
        menu.addSeparator();
    }
    QAction *pAddAction = menu.addAction("Add System Parameter");

    QCursor cursor;
    QAction *selectedAction = menu.exec(cursor.pos());
    if(selectedAction == pAddAction)
    {
        //! @todo maybe the gpSystemParametersWidget should not be global, should be one for each system
        gpSystemParametersWidget->openAddParameterDialog();
        return;
    }
    QString parNameString = actionParamMap.value(selectedAction);
    if(!parNameString.isEmpty())
    {
        mpValueEdit->setText(parNameString);
        refreshValueTextStyle();
    }
}

void ParameterValueSelectionWidget::openValueEditDialog()
{
    auto pTexteditorDialog = new QDialog(gpMainWindowWidget);
    pTexteditorDialog->setWindowTitle(QString("Edit %1").arg(mVariablePortDataName));
    auto pLayout = new QGridLayout(pTexteditorDialog);
    auto pTexteditor = new QTextEdit(pTexteditorDialog);
    pTexteditor->setWordWrapMode(QTextOption::NoWrap);
    pTexteditor->setPlainText(mpValueEdit->text());
    auto pButtonBox = new QDialogButtonBox(pTexteditorDialog);
    pButtonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(pButtonBox, SIGNAL(accepted()), pTexteditorDialog, SLOT(accept()));
    connect(pButtonBox, SIGNAL(rejected()), pTexteditorDialog, SLOT(reject()));

    auto *pHelpText = new QLabel(mpModelObject->getHelpText(), pTexteditorDialog);
    pHelpText->setWordWrap(true);

    pLayout->addWidget(pTexteditor, 0, 0, 1, 2);
    pLayout->addWidget(pHelpText, 0, 2, 1, 1, Qt::AlignRight);
    pLayout->addWidget(pButtonBox, 1, 0, 1, 3, Qt::AlignRight);

    pTexteditor->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred));
    pHelpText->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred));

    auto rc = pTexteditorDialog->exec();
    if (rc == QDialog::Accepted) {
        QString text = pTexteditor->toPlainText();
        // Increase max length if needed
        if (mpValueEdit->maxLength() < text.size()) {
            mpValueEdit->setMaxLength(text.size());
        }
        mpValueEdit->setText(text);
    }
}

void ParameterValueSelectionWidget::openFileBrowserDialog()
{
    QString defaultPath = gpDesktopHandler->getDocumentsPath();
    if(!mpValueEdit->text().isEmpty()) {
        defaultPath = QFileInfo(mpValueEdit->text()).absolutePath();
    }
    QString filePath = QFileDialog::getOpenFileName(gpMainWindowWidget, "Select file", defaultPath);
    if(!filePath.isEmpty()) {
        QFileInfo modelFileInfo(mpModelObject->getParentSystemObject()->getModelFilePath());
        mpValueEdit->setText(modelFileInfo.absoluteDir().relativeFilePath(filePath));
    }
}

void ParameterValueSelectionWidget::refreshValueTextStyle()
{
    if(mpModelObject && mpValueEdit)
    {
        if( !mCustomScale.isEmpty() || (mpValueEdit->text() != mpModelObject->getDefaultParameterValue(mVariablePortDataName)) )
        {
            QString style("color: black; font: bold;");
            decideBackgroundColor(style);
            mpValueEdit->setStyleSheet(style);
        }
        else
        {
            setDefaultValueTextStyle();
        }

    }
}

void ParameterValueSelectionWidget::rescaleByUnitScale(const UnitConverter &rUnitScale)
{
    if (mpValueEdit)
    {
        bool isNumericValue=false;
        mpValueEdit->text().toDouble(&isNumericValue);
        QString valS = mpValueEdit->text();
        if (isNumericValue)
        {
            // If we already have a custom scale then convert back to base unit first
            if (!mCustomScale.isEmpty())
            {
                valS = mDefaultUnitScale.convertFromBase(mCustomScale.convertToBase(valS));
            }
            // Now convert based on new scale values
            valS = rUnitScale.convertFromBase(mDefaultUnitScale.convertToBase(valS));

            // Set new value and remember new custom scale
            mpValueEdit->setText(QString("%1").arg(valS));
            mCustomScale = rUnitScale;
            refreshValueTextStyle();
        }
        else if (mpUnitSelectionWidget)
        {
            mpUnitSelectionWidget->resetDefault();
        }
    }
}

bool ParameterValueSelectionWidget::checkIfSysParEntered()
{
    bool syspar = false;
    if (mpValueEdit)
    {
        syspar = mpModelObject->getParentSystemObject()->hasParameter(mpValueEdit->text());
    }

    if (mpUnitSelectionWidget)
    {
        if (syspar)
        {
            mpUnitSelectionWidget->resetDefault();
        }
        mpUnitSelectionWidget->setDisabled(syspar);
    }
    return syspar;
}

void ParameterValueSelectionWidget::setDefaultValueTextStyle()
{
    if (mpValueEdit)
    {
        QString style("color: black; font: normal;");
        decideBackgroundColor(style);
        mpValueEdit->setStyleSheet(style);
    }
}

void ParameterValueSelectionWidget::decideBackgroundColor(QString &rStyleString)
{
    if (mpValueEdit)
    {
        if (mVariameterType == VariableTableWidget::InputVaraiable)
        {
            Port *pPort = mpModelObject->getPort(mVariablePortName);
            if (pPort && pPort->isConnected())
            {
                rStyleString.append(" color: gray; background: LightGray; font: italic;");
                return;
            }
        }
        rStyleString.append(" background: white;");
    }
}


HideShowPortWidget::HideShowPortWidget(const CoreVariameterDescription &rData, ModelObject *pModelObject, QWidget *pParent) :
    QWidget(pParent)
{
    mpModelObject = pModelObject;
    mPortName = rData.mPortName;

    QHBoxLayout *pLayout = new QHBoxLayout(this);
    QMargins margins = pLayout->contentsMargins(); margins.setBottom(0); margins.setTop(0);
    pLayout->setContentsMargins(margins);
    mpCheckBox = new QCheckBox(this);
    pLayout->addWidget(mpCheckBox,Qt::AlignRight);

    mpCheckBox->setToolTip("Show/hide port");
    Port *pPort = mpModelObject->getPort(mPortName);
    mpCheckBox->setChecked((pPort && pPort->getPortAppearance()->mEnabled));
    connect(mpCheckBox, SIGNAL(toggled(bool)), this, SLOT(hideShowPort(bool)));
    connect(mpCheckBox, SIGNAL(toggled(bool)), this, SIGNAL(toggled(bool)));

    mpCheckBox->setDisabled(pPort && pPort->isConnected());
}

void HideShowPortWidget::refreshPortToggleState()
{
    Port *pPort = mpModelObject->getPort(mPortName);
    QSignalBlocker scopedBlocker(mpCheckBox); // Prevent firing signal if toggled
    mpCheckBox->setChecked(pPort && pPort->getPortAppearance()->mEnabled);
}

QCheckBox *HideShowPortWidget::getCheckBoxPtr() const
{
    return mpCheckBox;
}

void HideShowPortWidget::hideShowPort(const bool doShow)
{
    if (doShow)
    {
        Port *pPort = mpModelObject->getPort(mPortName);
        if (pPort)
        {
            pPort->setEnable(true);
            mpModelObject->createRefreshExternalPort(mPortName);
        }
        else
        {
            pPort = mpModelObject->createRefreshExternalPort(mPortName);
            if (pPort)
            {
                // Make sure that our new port has the "correct" angle
                //! @todo this is incorrect, can not have hardcoded 180
                pPort->setRotation(180);
                pPort->setModified(true);
            }
        }
    }
    else
    {
        Port *pPort = mpModelObject->getPort(mPortName);
        if (pPort)
        {
            pPort->setEnable(false);
        }
    }
}


UnitSelectionWidget::UnitSelectionWidget(const QString &rQuantity, QWidget *pParent) :
    QWidget(pParent)
{
    mpUnitComboBox = nullptr;
    mQuantity = rQuantity;
    mBaseUnitIndex = -1;

    QHBoxLayout *pLayout = new QHBoxLayout(this);
    pLayout->setContentsMargins(0,0,0,0);

    gpConfig->getUnitScales(mQuantity, mUnitScales);
    mBaseUnit = gpConfig->getBaseUnit(mQuantity);

    if (!mUnitScales.isEmpty())
    {
        mpUnitComboBox = new QComboBox(this);
        mpUnitComboBox->installEventFilter(new MouseWheelEventEater(this));
        mpUnitComboBox->setMinimumWidth(60);

        for( UnitConverter &us : mUnitScales )
        {
            mpUnitComboBox->addItem(us.mUnit);
            if (mBaseUnitIndex < 0)
            {
                // check if this is the default unit
                if ( mBaseUnit == us.mUnit )
                {
                    mBaseUnitIndex = mpUnitComboBox->count()-1;
                }
            }
        }

        mpUnitComboBox->setCurrentIndex(mBaseUnitIndex);
        pLayout->addWidget(mpUnitComboBox);
        connect(mpUnitComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectionChanged(int)));
    }
}

void UnitSelectionWidget::setUnitScaling(const UnitConverter &rUs)
{
    if (mpUnitComboBox && !rUs.isEmpty())
    {
        int i = mpUnitComboBox->findText(rUs.mUnit);
        // If we have it, then make it selected
        if (i >= 0)
        {
            mpUnitComboBox->setCurrentIndex(i);
        }
        // If not then Add to list
        else
        {
            mUnitScales.append(rUs);
            mpUnitComboBox->addItem(rUs.mUnit);
            mpUnitComboBox->setCurrentIndex(mpUnitComboBox->count()-1);
        }
    }
}

QString UnitSelectionWidget::getSelectedUnit() const
{
    UnitConverter us;
    getSelectedUnitScale(us);
    return us.mUnit;
}

double UnitSelectionWidget::getSelectedUnitScale() const
{
    UnitConverter us;
    getSelectedUnitScale(us);
    return us.scaleToDouble();
}

void UnitSelectionWidget::getSelectedUnitScale(UnitConverter &rUnitScale) const
{
    if (mpUnitComboBox)
    {
        int i = mpUnitComboBox->currentIndex();
        if (i < mUnitScales.size())
        {
            rUnitScale =  mUnitScales[i];
        }
    }
    else
    {
        rUnitScale = UnitConverter(mQuantity, mBaseUnit, "1.0", "");
    }
}

bool UnitSelectionWidget::isDefaultSelected() const
{
    if (mpUnitComboBox)
    {
        return mpUnitComboBox->currentIndex() == mBaseUnitIndex;
    }
    else
    {
        return true;
    }
}

void UnitSelectionWidget::resetDefault()
{
    if (mpUnitComboBox)
    {
        mpUnitComboBox->setCurrentIndex(mBaseUnitIndex);
    }
}

void UnitSelectionWidget::selectionChanged(int idx)
{
    Q_UNUSED(idx)
    UnitConverter us;
    getSelectedUnitScale(us);
    emit unitChanged(us);
}


QuantitySelectionWidget::QuantitySelectionWidget(const CoreVariameterDescription &rData, ModelObject *pModelObject, QWidget *pParent) :
    QWidget(pParent)
{
    mVariableTypeName = rData.mName;
    mVariablePortDataName = rData.mPortName+"#"+rData.mName;
    mOriginalUnit = rData.mUnit;
    mQuantity = rData.mQuantity;
    mpModelObject = pModelObject;

    QHBoxLayout* pLayout = new QHBoxLayout(this);
    QMargins margins = pLayout->contentsMargins(); margins.setBottom(0); margins.setTop(0);
    pLayout->setContentsMargins(margins);

    mpQuantityLabel = new QLabel(this);
    mpQuantityLabel->setAlignment(Qt::AlignLeft);
    //mpQuantityLabel->setFrame(false);
    mpQuantityLabel->setText(mQuantity);
    pLayout->addWidget(mpQuantityLabel);

    if (rData.mUserModifiableQuantity)
    {
//        //! @todo read custom quantity maybe (but if it is set in core we wont need to read it since we already have iti
//        QString custQuant = mpModelObject->getCustomQuantity(mVariablePortDataName);
//        mpQuantityLabel->setText(custQuant);
//        mQuantity = custQuant;

        QToolButton *pQuantitySelectionButton =  new QToolButton(this);
        pQuantitySelectionButton->setIcon(QIcon(QString(ICONPATH) + "svg/Hopsan-NewPlot.svg"));
        pQuantitySelectionButton->setToolTip("Select Custom Quantity");
        pQuantitySelectionButton->setFixedSize(24,24);
        connect(pQuantitySelectionButton, SIGNAL(clicked()), this, SLOT(createQuantitySelectionMenu()));
        pLayout->addWidget(pQuantitySelectionButton);
    }
}

void QuantitySelectionWidget::registerCustomQuantity()
{
    if (hasChanged())
    {
        bool rc = mpModelObject->setModifyableSignalQuantity(mVariablePortDataName, mpQuantityLabel->text());
        if (rc)
        {
            mQuantity = mpQuantityLabel->text();
        }
    }
}

bool QuantitySelectionWidget::hasChanged() const
{
    return (mQuantity != mpQuantityLabel->text());
}

void QuantitySelectionWidget::createQuantitySelectionMenu()
{
    QStringList quantities = gpConfig->getUnitQuantities();

    QMenu menu;
    QAction *pClearAction = menu.addAction("Clear");
    menu.addSeparator();
    QMap<QAction*, int> actionScaleMap;
    for (int i=0; i<quantities.size(); ++i)
    {
        QAction *tempAction = menu.addAction(quantities[i]);
        actionScaleMap.insert(tempAction, i);
        tempAction->setIconVisibleInMenu(false);
    }

    QCursor cursor;
    QAction *pSelectedAction = menu.exec(cursor.pos());

    if (pSelectedAction == pClearAction)
    {
        mpQuantityLabel->clear();
        registerCustomQuantity();
    }
    else
    {
        int idx = actionScaleMap.value(pSelectedAction,-1);
        if (idx >= 0)
        {
            mpQuantityLabel->setText(quantities[idx]);
            registerCustomQuantity();
        }
    }
}

PlotSettingsWidget::PlotSettingsWidget(const CoreVariameterDescription &rData, ModelObject *pModelObject, QWidget *pParent)
    : QWidget(pParent)
{
    mpModelObject = pModelObject;
    mVariablePortDataName = rData.mPortName+"#"+rData.mName;
    mOrigInverted = mpModelObject->getInvertPlotVariable(mVariablePortDataName);
    mOriginalPlotLabel = mpModelObject->getVariablePlotLabel(mVariablePortDataName);

    QHBoxLayout *pLayout = new QHBoxLayout(this);
    QMargins margins = pLayout->contentsMargins(); margins.setBottom(0); margins.setTop(0);
    pLayout->setContentsMargins(margins);
    QCheckBox *pInverCheckbox = new QCheckBox("Inv", this);
    pInverCheckbox->setToolTip("Invert plot curve");
    pInverCheckbox->setChecked(mOrigInverted);
    mpPlotLabel = new QLineEdit(mOriginalPlotLabel,this);
    mpPlotLabel->setFrame(false);
    mpPlotLabel->setToolTip("Custom plot curve label");
    pLayout->addWidget(mpPlotLabel);
    pLayout->addWidget(pInverCheckbox);

    connect(pInverCheckbox, SIGNAL(toggled(bool)), this, SLOT(invertPlot(bool)));
    connect(mpPlotLabel, SIGNAL(textChanged(QString)), this, SLOT(setPlotLabel(QString)));
}

QLineEdit *PlotSettingsWidget::plotLabel()
{
    return mpPlotLabel;
}

void PlotSettingsWidget::invertPlot(bool tf)
{
    mpModelObject->setInvertPlotVariable(mVariablePortDataName, tf);
}

void PlotSettingsWidget::setPlotLabel(QString label)
{
    mpModelObject->setVariablePlotLabel(mVariablePortDataName, label);
}

SystemProperties::SystemProperties(SystemObject *pSystemObject, QWidget *pParentWidget) :
    QObject(pParentWidget)
{
    mpSystemObject = pSystemObject;
}

void SystemProperties::fixTimeStepInheritance(bool value)
{
    double ts;
    if(value)
    {
        ts = mpSystemObject->getParentSystemObject()->getCoreSystemAccessPtr()->getDesiredTimeStep();
    }
    else
    {
        ts = mpSystemObject->getCoreSystemAccessPtr()->getDesiredTimeStep();
    }
    mpTimeStepEdit->setText(QString("%1").arg(ts));
}

//! @brief Updates model settings according to the selected values
void SystemProperties::setValues()
{
    if(mpIsoCheckBox->isChecked() && mpSystemObject->getGfxType() != ISOGraphics)
    {
        mpSystemObject->setGfxType(ISOGraphics);
        gpLibraryWidget->setGfxType(ISOGraphics);
    }
    else if(!mpIsoCheckBox->isChecked() && mpSystemObject->getGfxType() != UserGraphics)
    {
        mpSystemObject->setGfxType(UserGraphics);
        gpLibraryWidget->setGfxType(UserGraphics);
    }

    mpSystemObject->getCoreSystemAccessPtr()->setKeepValuesAsStartValues(mpKeepValuesAsStartValues->isChecked());

    if(mpSystemObject->isUndoEnabled() != mpEnableUndoCheckBox->isChecked())
    {
        mpSystemObject->setUndoEnabled(mpEnableUndoCheckBox->isChecked());
    }

    mpSystemObject->setSaveUndo(mpSaveUndoCheckBox->isChecked());

    mpSystemObject->setAnimationDisabled(mpDisableAnimationCheckBox->isChecked());

    // Set the icon paths, only update and refresh appearance if a change has occurred
    AbsoluteRelativeEnumT absrel=Relative;
    if (isPathAbsolute(mpIsoIconPath->text()))
    {
        absrel=Absolute;
    }
    if ( mpSystemObject->getIconPath(ISOGraphics, absrel) != mpIsoIconPath->text() )
    {
        mpSystemObject->setIconPath(mpIsoIconPath->text(), ISOGraphics, absrel);
        mpSystemObject->refreshAppearance();
    }
    if ( mpSystemObject->getIconPath(UserGraphics, absrel) != mpUserIconPath->text() )
    {
        mpSystemObject->setIconPath(mpUserIconPath->text(), UserGraphics, absrel);
        mpSystemObject->refreshAppearance();
    }

    // Set scale if they have changed
    //! @todo maybe use fuzzy compare utility function instead (but then we need to include utilities here)
    if ( fabs(mpSystemObject->getAppearanceData()->getIconScale(ISOGraphics) - mpIsoIconScaleEdit->text().toDouble()) > 0.001 )
    {
        mpSystemObject->getAppearanceData()->setIconScale(mpIsoIconScaleEdit->text().toDouble(), ISOGraphics);
        mpSystemObject->refreshAppearance();
    }
    if ( fabs(mpSystemObject->getAppearanceData()->getIconScale(UserGraphics) - mpUserIconScaleEdit->text().toDouble()) > 0.001 )
    {
        mpSystemObject->getAppearanceData()->setIconScale(mpUserIconScaleEdit->text().toDouble(), UserGraphics);
        mpSystemObject->refreshAppearance();
    }

    // Set GuiSystem specific stuff
    mpSystemObject->getCoreSystemAccessPtr()->setInheritTimeStep(mpTimeStepCheckBox->isChecked());
    mpSystemObject->getCoreSystemAccessPtr()->setDesiredTimeStep(mpTimeStepEdit->text().toDouble());
    mpSystemObject->setNumberOfLogSamples(mpNumLogSamplesEdit->text().toInt());
    mpSystemObject->setLogStartTime(mpLogStartTimeEdit->text().toDouble());

    // Set model info
    mpSystemObject->setModelInfo(mpAuthorEdit->text(), mpEmailEdit->text(), mpAffiliationEdit->text(), mpDescriptionEdit->toPlainText());
}

//! @brief Slot that opens a file dialog where user can select a user icon for the system
void SystemProperties::browseUser()
{
    QString iconFileName = QFileDialog::getOpenFileName(gpMainWindowWidget, tr("Choose user icon"),
                                                        gpDesktopHandler->getModelsPath());
    if (!iconFileName.isEmpty())
    {
        mpUserIconPath->setText(iconFileName);
    }
}

//! @brief Slot that opens a file dialog where user can select an iso icon for the system
void SystemProperties::browseIso()
{
    QString iconFileName = QFileDialog::getOpenFileName(gpMainWindowWidget, tr("Choose ISO icon"),
                                                        gpDesktopHandler->getModelsPath());
    if (!iconFileName.isEmpty())
    {
        mpIsoIconPath->setText(iconFileName);
    }
}

void SystemProperties::clearLogData()
{
    QMessageBox existWarningBox(QMessageBox::Warning, "Warning","ALL log data in current model will be cleared. Continue?", nullptr, nullptr);
    existWarningBox.addButton("Yes", QMessageBox::AcceptRole);
    existWarningBox.addButton("No", QMessageBox::RejectRole);
    existWarningBox.setWindowIcon(gpMainWindowWidget->windowIcon());
    bool doIt = (existWarningBox.exec() == QMessageBox::AcceptRole);

    if(doIt)
    {
        mpSystemObject->getLogDataHandler()->clear();
    }
}

#include "ComponentPropertiesDialog3.moc"
