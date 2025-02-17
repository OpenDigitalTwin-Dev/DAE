/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

package org.scilab.forge.scirenderer.implementation.jogl;

import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.clipping.ClippingManager;
import org.scilab.forge.scirenderer.implementation.jogl.clipping.JoGLClippingManager;
import org.scilab.forge.scirenderer.implementation.jogl.drawer.JoGLShapeDrawer;
import org.scilab.forge.scirenderer.implementation.jogl.drawer.JoGLShapeDrawer.AntiAliasing;
import org.scilab.forge.scirenderer.implementation.jogl.lightning.JoGLLightManager;
import org.scilab.forge.scirenderer.lightning.LightManager;
import org.scilab.forge.scirenderer.renderer.Renderer;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.texture.AnchorPosition;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.tranformations.TransformationManager;
import org.scilab.forge.scirenderer.tranformations.TransformationManagerImpl;
import org.scilab.forge.scirenderer.tranformations.TransformationManagerListener;
import org.scilab.forge.scirenderer.tranformations.Vector3d;

import com.jogamp.opengl.GL2;

/**
 *
 * JoGl implementation of the DrawingTools.
 *
 * @author Pierre Lando
 */
public class JoGLDrawingTools implements DrawingTools {

    private final JoGLCapacity capacity = new JoGLCapacity();
    private final TransformationManager transformationManager;
    private final JoGLLightManager lightManager;
    private final JoGLClippingManager clippingManager;
    private final JoGLCanvas canvas;
    private GL2 gl;
    private int PixelDrawingMode;

    /**
     * Default constructor.
     * @param canvas the canvas where this drawing tools live.
     */
    JoGLDrawingTools(JoGLCanvas canvas) {
        this.transformationManager = new TransformationManagerImpl(canvas);
        this.lightManager = new JoGLLightManager(this);
        this.clippingManager = new JoGLClippingManager(this);

        this.canvas = canvas;

        this.PixelDrawingMode = GL2.GL_COPY;

        transformationManager.addListener(new TransformationManagerListener() {
            @Override
            public void transformationChanged(TransformationManager transformationManager) {
                gl.glMatrixMode(GL2.GL_MODELVIEW);
                if (transformationManager.isUsingSceneCoordinate()) {
                    gl.glLoadMatrixd(transformationManager.getTransformation().getMatrix(), 0);
                } else {
                    gl.glLoadMatrixd(transformationManager.getWindowTransformation().getMatrix(), 0);
                }
            }
        });
    }

    /**
     * Synchronise to the given OpenGl context.
     * @param gl the OpenGL context.
     */
    void glSynchronize(GL2 gl) {
        this.gl = gl;
        transformationManager.reset();
        capacity.glReload(gl);
        lightManager.reload();
        clippingManager.reload();
    }

    @Override
    public JoGLCanvas getCanvas() {
        return canvas;
    }

    /**
     * Return the OpenGl context.
     * @return the OpenGl context.
     */
    public GL2 getGl() {
        return gl;
    }

    /**
     * Return the OpenGl capacity of this canvas.
     * @return the OpenGl capacity of this canvas.
     */
    public JoGLCapacity getGLCapacity() {
        return capacity;
    }

    @Override
    public TransformationManager getTransformationManager() {
        return transformationManager;
    }

    @Override
    public LightManager getLightManager() {
        return lightManager;
    }

    @Override
    public ClippingManager getClippingManager() {
        return clippingManager;
    }

    public void setPixelDrawingMode(JoGLPixelDrawingMode mode) {
        switch (mode) {
            case CLEAR:
                PixelDrawingMode = GL2.GL_CLEAR;
                break;
            case AND:
                PixelDrawingMode = GL2.GL_AND;
                break;
            case AND_REVERSE:
                PixelDrawingMode = GL2.GL_AND_REVERSE;
                break;
            case COPY:
                PixelDrawingMode = GL2.GL_COPY;
                break;
            case AND_INVERTED:
                PixelDrawingMode = GL2.GL_AND_INVERTED;
                break;
            case NOOP:
                PixelDrawingMode = GL2.GL_NOOP;
                break;
            case XOR:
                PixelDrawingMode = GL2.GL_XOR;
                break;
            case OR:
                PixelDrawingMode = GL2.GL_OR;
                break;
            case NOR:
                PixelDrawingMode = GL2.GL_NOR;
                break;
            case EQUIV:
                PixelDrawingMode = GL2.GL_EQUIV;
                break;
            case INVERT:
                PixelDrawingMode = GL2.GL_INVERT;
                break;
            case OR_REVERSE:
                PixelDrawingMode = GL2.GL_OR_REVERSE;
                break;
            case COPY_INVERTED:
                PixelDrawingMode = GL2.GL_COPY_INVERTED;
                break;
            case OR_INVERTED:
                PixelDrawingMode = GL2.GL_OR_INVERTED;
                break;
            case NAND:
                PixelDrawingMode = GL2.GL_NAND;
                break;
            case SET:
                PixelDrawingMode = GL2.GL_SET;
                break;
            default:
                PixelDrawingMode = GL2.GL_COPY;
                break;
        }
    }

    public int getGLPixelDrawingMode() {
        return PixelDrawingMode;
    }

    @Override
    public void clear(Color color) {
        gl.glClearColor(color.getRedAsFloat(), color.getGreenAsFloat(), color.getBlueAsFloat(), 1.f);
        gl.glClear(GL2.GL_COLOR_BUFFER_BIT);
    }

    @Override
    public void clear(java.awt.Color color) {
        float[] colorData = color.getRGBColorComponents(null);
        gl.glClearColor(colorData[0], colorData[1], colorData[2], 1f);
        gl.glClear(GL2.GL_COLOR_BUFFER_BIT);
    }

    @Override
    public void clearDepthBuffer() {
        gl.glClear(GL2.GL_DEPTH_BUFFER_BIT);
    }

    @Override
    public void draw(Renderer renderer) {
        canvas.getRendererManager().draw(this, renderer);
    }

    @Override
    public void draw(Geometry geometry) throws SciRendererException {
        JoGLShapeDrawer.getDrawer().draw(this, geometry, Appearance.getDefault(), AntiAliasing.ON);
    }

    @Override
    public void draw(Geometry geometry, AntiAliasing eAntiAliasing) throws SciRendererException {
        JoGLShapeDrawer.getDrawer().draw(this, geometry, Appearance.getDefault(), eAntiAliasing);
    }

    @Override
    public void draw(Geometry geometry, Appearance appearance) throws SciRendererException {
        JoGLShapeDrawer.getDrawer().draw(this, geometry, appearance);
    }

    @Override
    public void draw(Geometry geometry, Appearance appearance, AntiAliasing eAntiAliasing) throws SciRendererException {
        JoGLShapeDrawer.getDrawer().draw(this, geometry, appearance, eAntiAliasing);
    }

    @Override
    public void draw(Texture texture) throws SciRendererException {
        canvas.getTextureManager().draw(this, texture);
    }

    @Override
    public void draw(Texture texture, AnchorPosition anchor, ElementsBuffer positions) throws SciRendererException {
        canvas.getTextureManager().draw(this, texture, anchor, positions, 0, 1, 0, null, null);
    }

    @Override
    public void draw(Texture texture, AnchorPosition anchor, ElementsBuffer positions, double rotationAngle) throws SciRendererException {
        canvas.getTextureManager().draw(this, texture, anchor, positions, 0, 1, rotationAngle, null, null);
    }

    @Override
    public void draw(Texture texture, AnchorPosition anchor, ElementsBuffer positions, Color auxColor, ElementsBuffer colors) throws SciRendererException {
        canvas.getTextureManager().draw(this, texture, anchor, positions, 0, 1, 0, auxColor, colors);
    }

    @Override
    public void draw(Texture texture, AnchorPosition anchor, ElementsBuffer positions, double rotationAngle, Color auxColor, ElementsBuffer colors) throws SciRendererException {
        canvas.getTextureManager().draw(this, texture, anchor, positions, 0, 1, rotationAngle, auxColor, colors);
    }

    @Override
    public void draw(Texture texture, AnchorPosition anchor, ElementsBuffer positions, int offset, int stride, double rotationAngle) throws SciRendererException {
        canvas.getTextureManager().draw(this, texture, anchor, positions, offset, stride, rotationAngle, null, null);
    }

    @Override
    public void draw(Texture texture, AnchorPosition anchor, ElementsBuffer positions, int offset, int stride, double rotationAngle, Color auxColor, ElementsBuffer colors) throws SciRendererException {
        canvas.getTextureManager().draw(this, texture, anchor, positions, offset, stride, rotationAngle, auxColor, colors);
    }

    @Override
    public void draw(Texture texture, AnchorPosition anchor, Vector3d position) throws SciRendererException {
        canvas.getTextureManager().draw(this, texture, anchor, position, 0);
    }

    @Override
    public void draw(Texture texture, AnchorPosition anchor, Vector3d position, double rotationAngle) throws SciRendererException {
        canvas.getTextureManager().draw(this, texture, anchor, position, rotationAngle);
    }

    /**
     * Bind the given texture to the OpenGl context.
     * @param texture the given texture.
     * @throws SciRendererException is thrown if the texture is invalid.
     */
    public void bind(Texture texture) throws SciRendererException {
        canvas.getTextureManager().bind(this, texture);
    }
}
