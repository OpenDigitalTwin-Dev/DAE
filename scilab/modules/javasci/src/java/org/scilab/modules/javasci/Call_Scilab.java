/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (https://www.swig.org).
 * Version 4.2.1
 *
 * Do not make changes to this file unless you know what you are doing - modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package org.scilab.modules.javasci;


import org.scilab.modules.types.ScilabTypeEnum;
import org.scilab.modules.types.ScilabIntegerTypeEnum;

 /**
   * Connector for Javasci v2.
   * This class should <b>not</b> be used directly
   * @see org.scilab.modules.javasci.Scilab
   * @author DIGITEO - Sylvestre LEDRU
   */
public class Call_Scilab {

 /**
   * Constructor
   */
 protected Call_Scilab() {
    throw new UnsupportedOperationException();
 }
  public static int Call_ScilabOpen(String SCIpath, boolean advancedMode, String ScilabStartup, int Stacksize) {
    return Call_ScilabJNI.Call_ScilabOpen(SCIpath, advancedMode, ScilabStartup, Stacksize);
  }

  public static boolean TerminateScilab(String ScilabQuit) {
    return Call_ScilabJNI.TerminateScilab(ScilabQuit);
  }

  public static int SendScilabJob(String job) {
    return Call_ScilabJNI.SendScilabJob(job);
  }

  public static int SendScilabJobs(String[] jobs, int numberjobs) {
    return Call_ScilabJNI.SendScilabJobs(jobs, numberjobs);
  }

  public static void SetFromJavaToON() {
    Call_ScilabJNI.SetFromJavaToON();
  }

  public static boolean isGraphicOpened() {
    return Call_ScilabJNI.isGraphicOpened();
  }

  public static int GetLastErrorCode() {
    return Call_ScilabJNI.GetLastErrorCode();
  }

  public static boolean isExistingVariable(String varname) {
    return Call_ScilabJNI.isExistingVariable(varname);
  }

  public static String getLastErrorMessage() {
    return Call_ScilabJNI.getLastErrorMessage();
  }

  public static ScilabTypeEnum getVariableType(String varname) {
    return ScilabTypeEnum.swigToEnum(Call_ScilabJNI.getVariableType(varname));
  }

  public static ScilabIntegerTypeEnum getIntegerPrecision(String varname) {
    return ScilabIntegerTypeEnum.swigToEnum(Call_ScilabJNI.getIntegerPrecision(varname));
  }

  public static boolean isComplex(String varname) {
    return Call_ScilabJNI.isComplex(varname);
  }

  public static int putString(String variableName, String[][] variable) {
    return Call_ScilabJNI.putString(variableName, variable);
  }

  public static int putDoubleComplex(String variableName, double[][] variable, double[][] imag) {
    return Call_ScilabJNI.putDoubleComplex(variableName, variable, imag);
  }

  public static int putDouble(String variableName, double[][] variable) {
    return Call_ScilabJNI.putDouble(variableName, variable);
  }

  public static int putBoolean(String variableName, boolean[][] variable) {
    return Call_ScilabJNI.putBoolean(variableName, variable);
  }

  public static int putByte(String variableName, byte[][] variable) {
    return Call_ScilabJNI.putByte(variableName, variable);
  }

  public static int putUnsignedByte(String variableName, byte[][] variable) {
    return Call_ScilabJNI.putUnsignedByte(variableName, variable);
  }

  public static int putShort(String variableName, short[][] variable) {
    return Call_ScilabJNI.putShort(variableName, variable);
  }

  public static int putUnsignedShort(String variableName, short[][] variable) {
    return Call_ScilabJNI.putUnsignedShort(variableName, variable);
  }

  public static int putInt(String variableName, int[][] variable) {
    return Call_ScilabJNI.putInt(variableName, variable);
  }

  public static int putUnsignedInt(String variableName, int[][] variable) {
    return Call_ScilabJNI.putUnsignedInt(variableName, variable);
  }

  public static int putSparse(String variableName, int nbRow, int nbCol, int[] nbRowItem, int[] colPos, double[] data) {
    return Call_ScilabJNI.putSparse(variableName, nbRow, nbCol, nbRowItem, colPos, data);
  }

  public static int putComplexSparse(String variableName, int nbRow, int nbCol, int[] nbRowItem, int[] colPos, double[] data, double[] imag) {
    return Call_ScilabJNI.putComplexSparse(variableName, nbRow, nbCol, nbRowItem, colPos, data, imag);
  }

  public static int putBooleanSparse(String variableName, int nbRow, int nbCol, int[] nbRowItem, int[] colPos) {
    return Call_ScilabJNI.putBooleanSparse(variableName, nbRow, nbCol, nbRowItem, colPos);
  }

  public static int putPolynomial(String variableName, String polyVarName, double[][][] data) {
    return Call_ScilabJNI.putPolynomial(variableName, polyVarName, data);
  }

  public static int putComplexPolynomial(String variableName, String polyVarName, double[][][] data, double[][][] imag) {
    return Call_ScilabJNI.putComplexPolynomial(variableName, polyVarName, data, imag);
  }

}
