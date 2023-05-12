#ifndef NGLSCENE_H_
#define NGLSCENE_H_

#include <ngl/Transformation.h>
#include <ngl/Vec3.h>
#include <ngl/BBox.h>
#include <QOpenGLWindow>
#include <QElapsedTimer>
#include <unordered_map>
#include <QSet>

#include "WindowParams.h"
#include "FirstPersonCamera.h"
#include <BlockSelector.h>

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
  /// @param [in] _numBlockSelector The number of block selectors to initialize.
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
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this is called everytime we want to draw the scene
  //----------------------------------------------------------------------------------------------------------------------
  void resizeGL(int _w, int _h) override;
  //----------------------------------------------------------------------------------------------------------------------

private:
  // variables
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
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief view position
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Mat4 m_view;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief projection
  //----------------------------------------------------------------------------------------------------------------------
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
  /// @brief creates an array out of clockSelector
  //----------------------------------------------------------------------------------------------------------------------
  std::vector <BlockSelector> m_BlockSelectorArray;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief 
  //----------------------------------------------------------------------------------------------------------------------
  std::unique_ptr<ngl::BBox> m_grid;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief 
  //----------------------------------------------------------------------------------------------------------------------
  std::unique_ptr<ngl::BBox> m_box;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief 
  //----------------------------------------------------------------------------------------------------------------------
  std::unique_ptr<ngl::BBox> m_bullet;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief 
  //----------------------------------------------------------------------------------------------------------------------
  QSet<Qt::Key> m_keysPressed;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief 
  //----------------------------------------------------------------------------------------------------------------------
  int m_moveTimer;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief 
  //----------------------------------------------------------------------------------------------------------------------
  int m_numBlockSelector;   
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief 
  //----------------------------------------------------------------------------------------------------------------------
  BlockSelector mySelector;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief 
  //----------------------------------------------------------------------------------------------------------------------
  BlockSelector mapBox;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief 
  //----------------------------------------------------------------------------------------------------------------------
  BlockSelector blockshoot;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief 
  //----------------------------------------------------------------------------------------------------------------------
  BlockSelector mapBlock[500];
  //----------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------------
  int m_numMap = 500;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief blockSelector array for the bullet blocks
  //----------------------------------------------------------------------------------------------------------------------
  BlockSelector bulletShot[1000];
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief setting value for number of bullets shot
  //----------------------------------------------------------------------------------------------------------------------
  int m_numBulletShot = 1000;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief blockSelector array for the enemy blocks
  //----------------------------------------------------------------------------------------------------------------------
  BlockSelector enemyBlocks[50];
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief setting value for number of enemies
  //----------------------------------------------------------------------------------------------------------------------
  int numOfEnemies = 50;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief blockSelector array for the barrier blocks
  //----------------------------------------------------------------------------------------------------------------------
  BlockSelector barrierBlocks[24];
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief setting value for barriers
  //----------------------------------------------------------------------------------------------------------------------
  int numOfBarriers = 24;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief 
  //----------------------------------------------------------------------------------------------------------------------
  float m_pickUpScale = 0.6f;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief 
  //----------------------------------------------------------------------------------------------------------------------
  int m_modeToDraw = 2;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief 
  //----------------------------------------------------------------------------------------------------------------------
  float m_globalBlockScale = 0.9f;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief 
  //----------------------------------------------------------------------------------------------------------------------
  int m_timeCount = 0;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief 
  //----------------------------------------------------------------------------------------------------------------------
  int m_timeIncrease = 0;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief 
  //----------------------------------------------------------------------------------------------------------------------
  float m_xEnemyLoc = -9;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief 
  //----------------------------------------------------------------------------------------------------------------------
  float m_zEnemyLoc = -6;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief 
  //----------------------------------------------------------------------------------------------------------------------
  int m_score = 0;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief 
  //----------------------------------------------------------------------------------------------------------------------
  int m_keyPressTimer = 0;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief 
  //----------------------------------------------------------------------------------------------------------------------
  int m_keyPauseSeprate = 0;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief 
  //----------------------------------------------------------------------------------------------------------------------
  int m_blockNumToShoot = 1;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief mod time count multiplier
  //----------------------------------------------------------------------------------------------------------------------
  int modTimeCount = 0;
  //----------------------------------------------------------------------------------------------------------------------
  int m_xEnemyDirection = 0;

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
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called everytime the mouse button is released
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void timerEvent(QTimerEvent *_event );
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief resets block selector
  //----------------------------------------------------------------------------------------------------------------------
  void resetBlockSelector();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief block selector collision
  //----------------------------------------------------------------------------------------------------------------------
  void checkBlockSelectorCollisions();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief collision test
  //----------------------------------------------------------------------------------------------------------------------
  void CollisionTest(BlockSelector* mapBlocko, int totalMapBlocks);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief checks collisions
  //----------------------------------------------------------------------------------------------------------------------
  int checkAABBCollision( const ngl::Vec3& box1Pos, float box1Width, float box1Depth, float box1Height, 
                          const ngl::Vec3& box2Pos, float box2Width, float box2Depth, float box2Height);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Draw a map block using the specified fill and outline colours and cube size.
  /// @param [in] fillColour The fill colour to use for the map block
  /// @param [in] outlineColour The outline colour to use for the map block
  /// @param [in] cubeSize The size of the cubes to draw
  //----------------------------------------------------------------------------------------------------------------------
  void mapBlockDraw(ngl::Vec4 fillColour, ngl::Vec4 outlineColour, float cubeSize);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Set the block selector object.
  /// @param [in] mapBlockot A pointer to the block selector object.
  /// @param [in] countNumBlock The number of block selectors to initialize.
  /// @param [in] fillColour The fill colour to use for the map block.
  /// @param [in] outlineColour The outline colour to use for the map block.
  /// @param [in] blockPosition The position of the block selector.
  //----------------------------------------------------------------------------------------------------------------------
  void mapBlockFunc(BlockSelector* mapBlockot, int countNumBlock, ngl::Vec4 fillColour, ngl::Vec4 outlineColour, ngl::Vec3 blockPosition);
  //----------------------------------------------------------------------------------------------------------------------
};


#endif
