/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (https://www.swig.org).
 * Version 4.2.1
 *
 * Do not make changes to this file unless you know what you are doing - modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package org.scilab.modules.scinotes;


 /**
   * @author Allan CORNET - DIGITEO 2009
   * @author Calixte DENIZET
   */
public class ScilabKeywords {

 /**
   * Constructor
   */
 protected ScilabKeywords() {
    throw new UnsupportedOperationException();
 }
  public static String[] GetVariablesName() {
    return ScilabKeywordsJNI.GetVariablesName();
  }

  public static String[] GetFunctionsName() {
    return ScilabKeywordsJNI.GetFunctionsName();
  }

  public static String[] GetMacrosName() {
    return ScilabKeywordsJNI.GetMacrosName();
  }

}
