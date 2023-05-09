#ifndef NGLSCENE_H_
#define NGLSCENE_H_
#include "WindowParams.h"
#include "FirstPersonCamera.h"
#include <ngl/Transformation.h>
#include <ngl/Vec3.h>
#include <QOpenGLWindow>
#include <QElapsedTimer>
#include <QSet>
#include <unordered_map>
#include <BlockSelector.h>
#include <ngl/BBox.h>



//----------------------------------------------------------------------------------------------------------------------
/// @file NGLScene.h
/// @brief this class inherits from the Qt OpenGLWindow and allows us to use NGL to draw OpenGL
/// @author Jonathan Macey
/// @version 1.0
/// @date 10/9/13
/// Revision History :
/// This is an initial version used for the new NGL6 / Qt 5 demos
/// @class NGLScene
/// @brief our main glwindow widget for NGL applications all drawing elements are
/// put in this file
//----------------------------------------------------------------------------------------------------------------------

class NGLScene : public QOpenGLWindow
{
  Q_OBJECT
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor for our NGL drawing class
  /// @param [in] parent the parent window to the class
  //----------------------------------------------------------------------------------------------------------------------
  NGLScene(int _numBlockSelector);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief dtor must close down ngl and release OpenGL resources
  //----------------------------------------------------------------------------------------------------------------------
  ~NGLScene() override;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the initialize class is called once when the window is created and we have a valid GL context
  /// use this to setup any default GL stuff
  //----------------------------------------------------------------------------------------------------------------------
  void initializeGL() override;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this is called everytime we want to draw the scene
  //----------------------------------------------------------------------------------------------------------------------
  void paintGL() override;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this is called everytime we want to draw the scene
  //----------------------------------------------------------------------------------------------------------------------
  // void UpdateForMove();
  // //----------------------------------------------------------------------------------------------------------------------
  // /// @brief this is called everytime we want to draw the scene
  // //----------------------------------------------------------------------------------------------------------------------
  void resizeGL(int _w, int _h) override;
  void mapBlockDraw(ngl::Vec4 fillColour,ngl::Vec4 outlineColour, float cubeSize);

  void mapBlockFunc(BlockSelector* mapBlockot, int countNumBlock, ngl::Vec4 fillColour, ngl::Vec4 outlineColour, ngl::Vec3 blockPosition);

  
  

private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the windows params such as mouse and rotations etc
  //----------------------------------------------------------------------------------------------------------------------
  WinParams m_win;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used to store the global mouse transforms
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Mat4 m_mouseGlobalTX;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Our Camera
  //----------------------------------------------------------------------------------------------------------------------
  FirstPersonCamera m_cam;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the model position for mouse movement
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_modelPos;
  ngl::Mat4 m_view;
  ngl::Mat4 m_project;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the model transformation
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Transformation m_transform;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief timing for camera update
  //----------------------------------------------------------------------------------------------------------------------
  float m_deltaTime = 0.0f;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief frame time for camera update
  //----------------------------------------------------------------------------------------------------------------------
  float m_lastFrame = 0.0f;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief timer for measurement
  /// -----------------------------------------------------------------------------
  QElapsedTimer m_timer;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the keys being pressed
  //----------------------------------------------------------------------------------------------------------------------
  std::vector <BlockSelector> m_BlockSelectorArray;
  std::unique_ptr<ngl::BBox> m_bbox;
  std::unique_ptr<ngl::BBox> m_bbox2;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief flag to indicate if we need to do spheresphere checks
    //----------------------------------------------------------------------------------------------------------------------
  QSet<Qt::Key> m_keysPressed;
  int m_moveTimer;
  int m_numBlockSelector;   
  // boxxx


  BlockSelector mySelector;
  BlockSelector mapBox;
  


  

  

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to load transform matrices to the shader
  //----------------------------------------------------------------------------------------------------------------------
  void loadMatricesToShader(ngl::Vec4 &_colour);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Qt Event called when a key is pressed
  /// @param [in] _event the Qt event to query for size etc
  //----------------------------------------------------------------------------------------------------------------------
  void keyPressEvent(QKeyEvent *_event) override;
  void keyReleaseEvent( QKeyEvent *_event	) override;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called every time a mouse is moved
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void mouseMoveEvent(QMouseEvent *_event) override;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called everytime the mouse button is pressed
  /// inherited from QObject and overridden here.
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void mousePressEvent(QMouseEvent *_event) override;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called everytime the mouse button is released
  /// inherited from QObject and overridden here.
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void mouseReleaseEvent(QMouseEvent *_event) override;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called everytime the mouse wheel is moved
  /// inherited from QObject and overridden here.
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void wheelEvent(QWheelEvent *_event) override;

  void timerEvent(QTimerEvent *_event );
  void resetBlockSelector();
  void checkBlockSelectorCollisions();
  void CollisionTest(BlockSelector* mapBlocko, int totalMapBlocks);
  

  float pickUpScale = 0.6f;
  int numMap = 2000;
  int modeToDraw = 2;
  float globalBlockScale = 0.9f;
  BlockSelector mapBlock[2000];
  int timeCount = 0;
  int timeIncrease = 0;
  float xEnemyLoc = -9;
  float zEnemyLoc = -6;
  int score = 0;
 


};


#endif
