/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (https://www.swig.org).
 * Version 4.2.1
 *
 * Do not make changes to this file unless you know what you are doing - modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package org.scilab.modules.xcos;

public class VectorOfBool {
  private transient long swigCPtr;
  protected transient boolean swigCMemOwn;

  protected VectorOfBool(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(VectorOfBool obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected static long swigRelease(VectorOfBool obj) {
    long ptr = 0;
    if (obj != null) {
      if (!obj.swigCMemOwn)
        throw new RuntimeException("Cannot release ownership as memory is not owned");
      ptr = obj.swigCPtr;
      obj.swigCMemOwn = false;
      obj.delete();
    }
    return ptr;
  }

  @SuppressWarnings({"deprecation", "removal"})
  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        JavaControllerJNI.delete_VectorOfBool(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public VectorOfBool() {
    this(JavaControllerJNI.new_VectorOfBool__SWIG_0(), true);
  }

  public VectorOfBool(long n) {
    this(JavaControllerJNI.new_VectorOfBool__SWIG_1(n), true);
  }

  public void ensureCapacity(long n) {
    JavaControllerJNI.VectorOfBool_ensureCapacity(swigCPtr, this, n);
  }

  public void resize(long n) {
    JavaControllerJNI.VectorOfBool_resize(swigCPtr, this, n);
  }

  public int size() {
    return JavaControllerJNI.VectorOfBool_size(swigCPtr, this);
  }

  public boolean isEmpty() {
    return JavaControllerJNI.VectorOfBool_isEmpty(swigCPtr, this);
  }

  public void clear() {
    JavaControllerJNI.VectorOfBool_clear(swigCPtr, this);
  }

  public void add(boolean x) {
    JavaControllerJNI.VectorOfBool_add__SWIG_0(swigCPtr, this, x);
  }

  public boolean get(int i) {
    return JavaControllerJNI.VectorOfBool_get(swigCPtr, this, i);
  }

  public void set(int i, boolean val) {
    JavaControllerJNI.VectorOfBool_set(swigCPtr, this, i, val);
  }

  public void add(int i, boolean val) {
    JavaControllerJNI.VectorOfBool_add__SWIG_1(swigCPtr, this, i, val);
  }

  public boolean remove(boolean val) {
    return JavaControllerJNI.VectorOfBool_remove(swigCPtr, this, val);
  }

}
