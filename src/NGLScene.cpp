#include "NGLScene.h"
#include "BlockSelector.h"
#include <QGuiApplication>
#include <QMouseEvent>
#include <SDL2/SDL.h>
#include <ngl/NGLInit.h>
#include <ngl/NGLStream.h>
#include <ngl/Random.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/VAOFactory.h>
#include <ngl/SimpleVAO.h>
#include <ngl/Obj.h>
#include <ngl/BBox.h>
#include <ngl/AABB.h>
#include <algorithm>
#include <iostream>
#include <QtGui/QPainter>

NGLScene::NGLScene(int _numBlockSelector)
{
  setTitle("Collision Game");
  m_timer.start();
  m_numBlockSelector = _numBlockSelector;
  resetBlockSelector();
}

void NGLScene::resetBlockSelector()
{
  m_BlockSelectorArray.resize(m_numBlockSelector);
  std::generate(std::begin(m_BlockSelectorArray), std::end(m_BlockSelectorArray), [this](){return BlockSelector();});
}

// tests if blocks collide
void NGLScene::CollisionTest(BlockSelector* mapBlocko, int totalMapBlocks)
{
  mySelector.moveNextPosition();
  int checkResult = 0;

  int blockPickedUp = 0;


  for (int j = 0; j < totalMapBlocks; j++) {
    if (mySelector.nextPosition == mapBlocko[j].position) 
    {
      checkResult = 1;
      std::cout<<"Collision detected at: ("<<mapBlocko[j].position.m_x<<", "<<mapBlocko[j].position.m_y<<", "<<mapBlocko[j].position.m_z<<")"<<"\n"; 
      blockPickedUp = j;
      score = score + 40;
    }
    else 
    {
      // checkResult = 0;
    }
  }

  if (checkResult == 1) 
  {
      if (mapBlocko[blockPickedUp].isPickup == 1) {
        mapBlocko[blockPickedUp].hasBeenPickedUp = 1;
        std::cout<<"Picked up block"<<"\n";
        mySelector.move();
        m_box->setCenter(mySelector.position);
      }
    // std::cout<<"collisions detected
  }
  else 
  {
    // std::cout<<"collisions not detected"<<"\n";
    mySelector.move();
    m_box->setCenter(mySelector.position);
  }
}

// draws the base grid of blocks
void NGLScene::mapBlockDraw(ngl::Vec4 fillColour,ngl::Vec4 outlineColour, float cubeSize)
{
        m_grid->width(cubeSize);
        m_grid->depth(cubeSize);
        m_grid->height(cubeSize);
        if (modeToDraw == 2){
          m_grid->setDrawMode(GL_FILL);
          loadMatricesToShader(fillColour);
          m_grid->draw();
        } else if (modeToDraw == 3){
          m_grid->setDrawMode(GL_LINE);
          loadMatricesToShader(outlineColour);
          m_grid->draw();
        } else {
          m_grid->setDrawMode(GL_FILL);
          loadMatricesToShader(fillColour);
          m_grid->draw();
          m_grid->setDrawMode(GL_LINE);
          loadMatricesToShader(outlineColour);
          m_grid->draw();
        }
}

  void NGLScene::mapBlockFunc(BlockSelector* mapBlockot, int countNumBlock, ngl::Vec4 fillColour, ngl::Vec4 outlineColour, ngl::Vec3 blockPosition) 
  {

    if (mapBlockot[countNumBlock].hasBeenPickedUp == 0) {
      mapBlockot[countNumBlock].position  = ngl::Vec3(blockPosition);
      m_grid->setCenter(mapBlockot[countNumBlock].position);
      mapBlockDraw(fillColour, outlineColour, mapBlockot[countNumBlock].blockScale);
    } 
    else {
      mapBlockot[countNumBlock].position  = ngl::Vec3(0,-1000,0);
      m_grid->setCenter(mapBlockot[countNumBlock].position);
      // mapBlockDraw(fillColour, outlineColour, mapBlockot[countNumBlock].blockScale);
    } 
  }

NGLScene::~NGLScene()
{
  std::cout << "Shutting down NGL, removing VAO's and Shaders\n";
}

void NGLScene::resizeGL(int _w, int _h)
{
  m_cam.setProjection(75.0f, static_cast<float>(_w) / _h, 0.05f, 350.0f);
  m_win.width = static_cast<int>(_w * devicePixelRatio());
  m_win.height = static_cast<int>(_h * devicePixelRatio());
}

void NGLScene::initializeGL()
{

  m_moveTimer = startTimer(20);
  // we must call that first before any other GL commands to load and link the
  // gl commands from the lib, if that is not done program will crash
  ngl::NGLInit::initialize();

  glClearColor(0.57f, 0.82f, 0.86f, 1.0f); // White Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
// enable multisampling for smoother drawing
#ifndef USINGIOS_
  glEnable(GL_MULTISAMPLE);

#endif

  // static camera
  // camera poistion
  ngl::Vec3 from(0, 5, 20);
  ngl::Vec3 to(0, 0, 0);
  ngl::Vec3 up(0, 1, 0);
  // loads new camers
  m_cam.set(from, to, up);
  // set the shape using FOV 90 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam.setProjection(90.0f, 1920.0f / 1080.0f, 0.05f, 350.0f);

  ngl::ShaderLib::use(ngl::nglColourShader);
  ngl::ShaderLib::setUniform("Colour", 1.0f, 1.0f, 1.0f, 1.0f);
  ngl::VAOPrimitives::createLineGrid("plane", 24, 24, 24);


  // boxes for the grid
  m_grid = std::make_unique<ngl::BBox>(ngl::Vec3(0.0f, 0.0f, 0.0f), mapBox.blockScale, mapBox.blockScale, mapBox.blockScale);

  // player character
  m_box = std::make_unique<ngl::BBox>(ngl::Vec3(0.0f, 0.0f, 0.0f), mySelector.blockScale, mySelector.blockScale, mySelector.blockScale);

  // bullet
  m_bullet = std::make_unique<ngl::BBox>(ngl::Vec3(0.0f, 0.0f, 0.0f), blockshoot.blockScale, blockshoot.blockScale, blockshoot.blockScale);
  mySelector.position = {ngl::Vec3(0.0f,0.0f,10.0f)};


  for (int u = 0; u < numMap; u++){
    mapBlock[u].position = ngl::Vec3(0.0f,-1000.0f,0.0f);
  }

  for (int u = 0; u < numBlockShoto; u++){
    blockshoto[u].position = ngl::Vec3(0.0f,-1000.0f,0.0f);
  }
  
}

void NGLScene::loadMatricesToShader(ngl::Vec4 &_colour)
{
  ngl::Mat4 MVP = m_cam.getVP() * m_transform.getMatrix();
  ngl::ShaderLib::setUniform("MVP", MVP);
  ngl::ShaderLib::setUniform("Colour", _colour);
}


void NGLScene::paintGL()
{
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  float currentFrame = m_timer.elapsed() * 0.001f;
  // std::cout << "Current Frame " << currentFrame << '\n';
  m_deltaTime = currentFrame - m_lastFrame;
  m_lastFrame = currentFrame;

  glViewport(0, 0, m_win.width, m_win.height);
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  ngl::ShaderLib::use(ngl::nglColourShader);

  /// first we reset the movement values
  float xDirection = 0.0;
  float yDirection = 0.0;
  float zDirection = 0.0;
  int keyPressed = 0;

  // if the set is non zero size we can update the ship movement
  // then tell openGL to re-draw
  if (m_keysPressed.size() != 0)
  {
    m_cam.move(xDirection, yDirection, zDirection, m_deltaTime);
  }

  int nrRows = 20;
  int nrColumns = 40;
  float offsetSpacing = 0.5;
  float spacing = 1.0;
  ngl::Random::setSeed(1234);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // setting colours
  ngl::Vec4 red(1.0f, 0.0f, 0.0f, 1.0f);
  ngl::Vec4 green(0.0f, 1.0f, 0.0f, 1.0f);
  ngl::Vec4 blue(0.0f, 0.0f, 1.0f, 1.0f);

  ngl::Vec4 white(1.0f, 1.0f, 1.0f, 1.0f);
  ngl::Vec4 darkGray(0.2f, 0.2f, 0.2f, 1.0f);
  ngl::Vec4 black(0.0f, 0.0f, 0.0f, 1.0f);
  
  ngl::Vec4 yellow(0.99f, 0.93f, 0.37f, 1.0f);
  ngl::Vec4 lightBlue(0.73f, 0.93f, 0.94f, 1.0f);
  ngl::Vec4 lilac(0.76f, 0.67f, 0.86f, 1.0f);
  ngl::Vec4 pink(0.95f, 0.79f, 0.88f, 1.0f);
  ngl::Vec4 appleGreen(0.8f, 0.95f, 0.79f, 1.0f);
  ngl::Vec4 grassGreen(0.34f, 0.74f, 0.57f, 1.0f);
  ngl::Vec4 orange(0.96f, 0.74f, 0.60f, 1.0f);
  ngl::Vec4 lightRed(1.0f, 0.55f, 0.55f, 1.0f);



  int currentFrameClamp = static_cast<int>(currentFrame*10);
  int FrameRim = currentFrameClamp % 20;
  

  if (timeCount == 0) 
  {
    timeIncrease = 1;
  }
  else if (timeCount == 100) {
    timeIncrease = 0;
  }

  if (timeIncrease == 0) {
    timeCount --;
  }
  else {
    timeCount ++;
  }

  keyPressTimer = timeCount % 4;

  std::cout<<"time count = "<<timeCount<<" keyPauseSeprate = "<<keyPressTimer<< "\n";
  

  
  int countNumBlock = 1;
  
  for (int row = 0; row < nrRows; ++row)
  {
    for (int col = 0; col < nrColumns; ++col)
    {
        mapBlockFunc(mapBlock, countNumBlock, appleGreen, white, ngl::Vec3(static_cast<float>(col - (nrColumns / 2)) * spacing, -1, static_cast<float>(row - (nrRows / 2)) * spacing));
        countNumBlock++;
    }
   }

    // mapBlock[countNumBlock].blockScale = pickUpScale;
    // mapBlockFunc(mapBlock, countNumBlock, orange, black, ngl::Vec3(3,0,4));
    // mapBlock[countNumBlock].isPickup = 1;
    // countNumBlock++;

    
    // mapBlock[countNumBlock].blockScale = pickUpScale;
    // mapBlockFunc(mapBlock, countNumBlock, orange, black, ngl::Vec3(-9,0,0));
    // mapBlock[countNumBlock].isPickup = 1;
    // countNumBlock++;

    // mapBlock[countNumBlock].blockScale = pickUpScale;
    // mapBlockFunc(mapBlock, countNumBlock, orange, black, ngl::Vec3(-7,0,-7));
    // mapBlock[countNumBlock].isPickup = 1;
    // countNumBlock++;

    // start of enemy blocks
    // offset in x direction
    int enemyXOffset = 2;
    // offset in y direction
    int enemyYOffset = 0;
    // speed that enemy blocks move in
    float enemySpeed = 0.01f;

    // moves enemies based on time count
    if (timeCount == 20) 
    {
      zEnemyLoc++;
    }
    // combining two if statements using the ternary operator ?
    xEnemyLoc += (timeCount >= 0 && timeCount < 50) ? enemySpeed : -enemySpeed;

    // changes the number of enemies on screen
    int numOfEnemies = 8;
    for (int j = 0; j < 2; j++) // add another loop for the second row
    {
      for (int i = 0; i < numOfEnemies; i++)
      {
        mapBlockFunc(mapBlock, countNumBlock, yellow, white, ngl::Vec3(xEnemyLoc + enemyXOffset, 0,zEnemyLoc + enemyYOffset));
        countNumBlock++;
        enemyXOffset += 2;
      }
      // reset x offset for the next row
      enemyXOffset = 2;
      // offset y for the next row
    }


  for (int o = 0; o < numBlockShoto; o++) 
  {
    // checks if block if fired
    if (blockshoto[o].hasBeenFired == 1) 
    {
      // updates position of blockshoto if fired
      blockshoto[o].position.m_z = blockshoto[o].position.m_z - 0.2f;
      mapBlockFunc(blockshoto, o, red, white, ngl::Vec3(blockshoto[o].xStartPosition, 0, blockshoto[o].position.m_z));
      
      // check if blockshoto has reached a certain position and delete it if it has
      if (blockshoto[o].position.m_z < -10.0f) 
      {
        blockshoto[o].position.m_z = 10.0f;
        blockshoto[o].position.m_y = -100.0f;
        blockshoto[o].hasBeenFired = 0;
      }
    }
    // else if the blockshoto has not been fired, set it to a position off screen
    else 
    {
      blockshoto[o].position.m_z = 10.0f;
      blockshoto[o].position.m_y = -100.0f;
    }
  }



  for (int i = 0; i < 3; i++) 
  {
    ngl::Vec3 position = ngl::Vec3(i * 2 - 2, 0, 2); // calculate the position of the block
    mapBlockFunc(mapBlock, countNumBlock + i, orange, black, position); // place the block
  }
  

  int CollsionTestTrue;

  // setting key controls
  foreach (Qt::Key key, m_keysPressed)
  {
    switch (key)
    {
    // moves player's character right
    case Qt::Key_Right:
    {
      mySelector.displacement = {ngl::Vec3(mySelector.displacementAmount,0.0f,0.0f)};
      keyPressed = 1;
      mySelector.moveNextPosition();
      CollisionTest(mapBlock, numMap);
      break;
    }
    // moves player's character left
    case Qt::Key_Left:
    {
      mySelector.displacement = {ngl::Vec3(-mySelector.displacementAmount,0.0f,0.0f)};
      keyPressed = 1;
      mySelector.moveNextPosition();
      CollisionTest(mapBlock, numMap);
      break;
    }
    // shoots blocks with spacebar
    case Qt::Key_Space: 
    {
      if (keyPressTimer == 0) 
      {
        blockshoto[blockNumToShoot].xStartPosition = mySelector.position.m_x;
        blockshoto[blockNumToShoot].position.m_z = 10;
        blockshoto[blockNumToShoot].hasBeenFired = 1;

        if (blockNumToShoot < 9) 
          {
            blockNumToShoot++;
          } 
          else 
          {
            blockNumToShoot = 1;
          }
      }
      break;
    }

    // camera movements
    case Qt::Key_A:
    {
      yDirection = -5.0f;
      break;
    }
    case Qt::Key_D:
    { 
      yDirection = 5.0f;
      break;
    }
    case Qt::Key_W:
    {
      xDirection = 5.0f;
      break;
    }
    case Qt::Key_S:
    {
      xDirection = -5.0f;
      break;
    }
    case Qt::Key_Shift:
    {
      zDirection = -5.0f;
      break;
    }
    case Qt::Key_CapsLock:
    {
      zDirection = +5.0f;
      break;
    }

    // wireframe modes 1 = points, 2 = lines, 3 = fill
    case Qt::Key_1:
    {
      modeToDraw = 1;
      break;
    }
    case Qt::Key_2:
    {
      modeToDraw = 2;
      break;
    }
    case Qt::Key_3:
    {
      modeToDraw = 3;
      break;
    }

    // block scale controls
    case Qt::Key_4:
    {
      if (mapBlock[1].blockScale >= 0) 
      {
        for (int u = 0; u < numMap; u++)
        {
          mapBlock[u].blockScale = mapBlock[u].blockScale - 0.01f;
        }
      }
      break;
    }
    case Qt::Key_5:
    {
      for (int u = 0; u < numMap; u++)
      {
        mapBlock[u].blockScale = mapBlock[u].blockScale + 0.01f;
      }
      break;
    }
    case Qt::Key_6:
    {
      for (int u = 0; u < numMap; u++)
      {
        mapBlock[u].blockScale = 0.9f;
      }
      break;
    }
    case Qt::Key_7:
    {
      for (int u = 0; u < numMap; u++)
      {
        mapBlock[u].blockScale = 1.0f;
      }
      break;
    }
    default:
    break;
    }
    update();
  }
    if (m_keysPressed.size() != 0)
  {
    m_cam.move(xDirection, yDirection, zDirection, m_deltaTime);
  }

glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
m_transform.reset();
if (FrameRim < 2 || (FrameRim >= 10 && FrameRim <= 12)) 
{
  if (modeToDraw == 2)
  {
    m_box->setDrawMode(GL_FILL);
    loadMatricesToShader(yellow);
    m_box->draw();
  } 
  else if (modeToDraw == 3)
  {
    m_box->setDrawMode(GL_LINE);
    loadMatricesToShader(black);
    m_box->draw();
  } 
  else 
  {
    m_box->setDrawMode(GL_FILL);
    loadMatricesToShader(yellow);
    m_box->draw();
    m_box->setDrawMode(GL_LINE);
    loadMatricesToShader(black);
    m_box->draw();
  }
  }
  else 
  {
  if (modeToDraw == 2)
  {
    m_box->setDrawMode(GL_FILL);
    loadMatricesToShader(lightRed);
    m_box->draw();
  } 
  else if (modeToDraw == 3)
  {
    m_box->setDrawMode(GL_LINE);
    loadMatricesToShader(white);
    m_box->draw();
  } 
  else 
  {
    m_box->setDrawMode(GL_FILL);
    loadMatricesToShader(lightRed);
    m_box->draw();
    m_box->setDrawMode(GL_LINE);
    loadMatricesToShader(white);
    m_box->draw();
  }
  }

  CollsionTestTrue = 0;
  m_transform.reset();

// create a new painter
  QPainter painter(this);

  // set the composition mode to only affect the text
  painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

  // set the font and pen color for the text
  painter.setFont(QFont("Helvetica", 22));
  painter.setPen(Qt::white);

  // draw the text
  painter.drawText(QPoint(12, 40), "Score: " + QString::number(score));
}

//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  // add to our keypress set the values of any keys pressed
  m_keysPressed += static_cast<Qt::Key>(_event->key());
  // that method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  auto setLight = [](std::string _num, bool _mode)
  {
    ngl::ShaderLib::use("PBR");
    if (_mode == true)
    {
      ngl::Vec3 colour = {255.0f, 255.0f, 255.0f};
      ngl::ShaderLib::setUniform(_num, colour);
    }
    else
    {
      ngl::Vec3 colour = {0.0f, 0.0f, 0.0f};
      ngl::ShaderLib::setUniform(_num, colour);
    }
  };
  switch (_event->key())
  {
  // escape key to quit
  case Qt::Key_Escape:
    QGuiApplication::exit(EXIT_SUCCESS);
    break;

// turn on wireframe rendering
#ifndef USINGIOS_
  case Qt::Key_W:
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    break;
  // turn off wire frame
  case Qt::Key_S:
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    break;
#endif
  // show full screen
  case Qt::Key_F:
    showFullScreen();
    break;
  // show windowed
  // case Qt::Key_N:
  //   showNormal();
  //   break;
  // case Qt::Key_Space:
  //   m_win.spinXFace = 0;
  //   m_win.spinYFace = 0;
  //   m_modelPos.set(ngl::Vec3::zero());
  //   break;
  default:
    break;
  }
  update();
}

void NGLScene::keyReleaseEvent(QKeyEvent *_event)
{
  // remove from our key set any keys that have been released
  m_keysPressed -= static_cast<Qt::Key>(_event->key());
}

void NGLScene::timerEvent(QTimerEvent *_event) {
 if (_event->timerId() == m_moveTimer)
  {
    // ++m_teapotRotation;
    // // re-draw GL
    update();
  }

}

