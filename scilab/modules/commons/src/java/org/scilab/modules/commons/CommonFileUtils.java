/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (https://www.swig.org).
 * Version 4.2.1
 *
 * Do not make changes to this file unless you know what you are doing - modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package org.scilab.modules.commons;


 /** 
   * Some file utils
   * @author Calixte DENIZET
   * @copyright DIGITEO 2011
   */
public class CommonFileUtils {

 /**
   * Constructor
   */
 protected CommonFileUtils() {
    throw new UnsupportedOperationException();
 }
  public static int isEmptyDirectory(String dirName) {
    return CommonFileUtilsJNI.isEmptyDirectory(dirName);
  }

  
 /**
   * Get the current working directory
   * @return the Scilab CWD
   */
public static String getCWD() {
    return CommonFileUtilsJNI.getCWD();
  }

}
