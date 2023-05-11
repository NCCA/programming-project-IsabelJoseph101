#include <iostream>
#include <algorithm>
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
#include <QGuiApplication>
#include <QMouseEvent>
#include <QtGui/QPainter>

#include "NGLScene.h"
#include "BlockSelector.h"

NGLScene::NGLScene(int _numBlockSelector)
{
  setTitle("Cosmic Crusaders");
  m_timer.start();
  m_numBlockSelector = _numBlockSelector;
  resetBlockSelector();
}

void NGLScene::resetBlockSelector()
{
  m_BlockSelectorArray.resize(m_numBlockSelector);
  std::generate(std::begin(m_BlockSelectorArray), std::end(m_BlockSelectorArray), [this](){return BlockSelector();});
}

int NGLScene::checkAABBCollision( const ngl::Vec3& box1Pos, float box1Width, float box1Depth, float box1Height, 
                                  const ngl::Vec3& box2Pos, float box2Width, float box2Depth, float box2Height)
{
    // calculate the minimum and maximum positions of each box
    ngl::Vec3 box1Min = box1Pos - ngl::Vec3(box1Width / 2, box1Height / 2, box1Depth / 2);
    ngl::Vec3 box1Max = box1Pos + ngl::Vec3(box1Width / 2, box1Height / 2, box1Depth / 2);
    ngl::Vec3 box2Min = box2Pos - ngl::Vec3(box2Width / 2, box2Height / 2, box2Depth / 2);
    ngl::Vec3 box2Max = box2Pos + ngl::Vec3(box2Width / 2, box2Height / 2, box2Depth / 2);

    // check for overlap along each axis
    if (box1Min.m_x <= box2Max.m_x && box1Max.m_x >= box2Min.m_x &&
        box1Min.m_y <= box2Max.m_y && box1Max.m_y >= box2Min.m_y &&
        box1Min.m_z <= box2Max.m_z && box1Max.m_z >= box2Min.m_z)
    {
        // boxes are colliding
        std::cout<<"Collision detected ---------------------------------------------"<<"\n"; 
        return 1;
    } 
    else 
    {
      // std::cout<<"Collision not detected"<<"\n"; 
      return 0;
    }
    // boxes are not colliding
  }
// tests if blocks collide
void NGLScene::CollisionTest(BlockSelector* mapBlocko, int totalMapBlocks)
{
  mySelector.moveNextPosition();
  int checkResult = 0;

  int blockPickedUp = 0;


  for (int j = 0; j < totalMapBlocks; j++) {
    if (mySelector.m_nextPosition == mapBlocko[j].m_position) 
    {
      checkResult = 1;
      std::cout<<"Collision detected at: ("<<mapBlocko[j].m_position.m_x<<", "<<mapBlocko[j].m_position.m_y<<", "<<mapBlocko[j].m_position.m_z<<")"<<"\n"; 
      blockPickedUp = j;
      m_score = m_score + 40;
    }
    else 
    {
      // checkResult = 0;
    }
  }

  if (checkResult == 1) 
  {
      if (mapBlocko[blockPickedUp].m_isPickup == 1) {
        mapBlocko[blockPickedUp].m_hasBeenPickedUp = 1;
        std::cout<<"Picked up block"<<"\n";
        mySelector.move();
        m_box->setCenter(mySelector.m_position);
      }
    // std::cout<<"collisions detected
  }
  else 
  {
    // std::cout<<"collisions not detected"<<"\n";
    mySelector.move();
    m_box->setCenter(mySelector.m_position);
  }
}

// draws the base grid of blocks
void NGLScene::mapBlockDraw(ngl::Vec4 fillColour,ngl::Vec4 outlineColour, float cubeSize)
{
  m_grid->width(cubeSize);
  m_grid->depth(cubeSize);
  m_grid->height(cubeSize);
  if (m_modeToDraw == 2){
    m_grid->setDrawMode(GL_FILL);
    loadMatricesToShader(fillColour);
    m_grid->draw();
  } else if (m_modeToDraw == 3){
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

// pick up objects
  void NGLScene::mapBlockFunc(BlockSelector* mapBlockot, int countNumBlock, ngl::Vec4 fillColour, ngl::Vec4 outlineColour, ngl::Vec3 blockPosition) 
  {

    if (mapBlockot[countNumBlock].m_hasBeenPickedUp == 0) {
      mapBlockot[countNumBlock].m_position  = ngl::Vec3(blockPosition);
      m_grid->setCenter(mapBlockot[countNumBlock].m_position);
      mapBlockDraw(fillColour, outlineColour, mapBlockot[countNumBlock].m_blockScale);
    } 
    else {
      mapBlockot[countNumBlock].m_position  = ngl::Vec3(0,-1000,0);
      m_grid->setCenter(mapBlockot[countNumBlock].m_position);
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


  // boxes for the grid using smart pointers
  m_grid = std::make_unique<ngl::BBox>(ngl::Vec3(0.0f, 0.0f, 0.0f), mapBox.m_blockScale, mapBox.m_blockScale, mapBox.m_blockScale);

  // player character using smart pointers
  m_box = std::make_unique<ngl::BBox>(ngl::Vec3(0.0f, 0.0f, 0.0f), mySelector.m_blockScale, mySelector.m_blockScale, mySelector.m_blockScale);

  // bullet using smart pointers 
  m_bullet = std::make_unique<ngl::BBox>(ngl::Vec3(0.0f, 0.0f, 0.0f), blockshoot.m_blockScale, blockshoot.m_blockScale, blockshoot.m_blockScale);
  mySelector.m_position = {ngl::Vec3(0.0f,0.0f,10.0f)};


  for (int u = 0; u < m_numMap; u++){
    mapBlock[u].m_position = ngl::Vec3(0.0f,-1000.0f,0.0f);
  }

  // initialising bullet locations
  for (int u = 0; u < m_numBulletShot; u++){
    bulletShot[u].m_position = ngl::Vec3(0.0f,-100.0f,0.0f);
    // makes bullet blocks smaller
    bulletShot[u].m_blockScale = 0.4;
  }

  // initialising enemy locations
  for (int t = 0; t < numOfEnemies; t++)
  {
    enemyBlocks[t].m_position = ngl::Vec3(0, -99, 0); 
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

  //----------------------------------------------------------------------------------------------------------------------
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
  ngl::Vec4 orange(1.0f, 0.81f, 0.31f, 1.0f);
  ngl::Vec4 lightRed(1.0f, 0.55f, 0.55f, 1.0f);
  //----------------------------------------------------------------------------------------------------------------------


  int currentFrameClamp = static_cast<int>(currentFrame*10);
  int FrameRim = currentFrameClamp % 20;
  

  if (m_timeCount == 0) 
  {
    m_timeIncrease = 1;
  }
  else if (m_timeCount == 100) {
    m_timeIncrease = 0;
  }

  if (m_timeIncrease == 0) {
    m_timeCount --;
  }
  else {
    m_timeCount ++;
  }
  

  m_keyPressTimer = m_timeCount % 4;

  // std::cout<<"time count = "<<m_timeCount<<" keyPauseSeprate = "<<m_keyPressTimer<< "\n";

  int countNumBlock = 1;

  // define the start and end hues for the gradient
  float startHue = 0.0f;
  float endHue = 300.0f;

  // define the reduced saturation and increased brightness for the pastel effect
  float saturation = 0.5f;
  float value = 1.2f;

  for (int row = 0; row < nrRows; ++row)
  {
    for (int col = 0; col < nrColumns; ++col)
    {
      // calculate the hue for this block based on its position in the grid
      float t = static_cast<float>(col) / static_cast<float>(nrColumns - 1);
      float hue = startHue + (t * (endHue - startHue));

      // calculate the RGB values for the pastel color
      float c = saturation * value;
      float x = c * (1.0f - std::abs(std::fmod(hue / 60.0f, 2.0f) - 1.0f));
      float m = value - c;
      float r, g, b;
      if (hue < 60.0f) {
        r = c;
        g = x;
        b = 0.0f;
      } else if (hue < 120.0f) {
        r = x;
        g = c;
        b = 0.0f;
      } else if (hue < 180.0f) {
        r = 0.0f;
        g = c;
        b = x;
      } else if (hue < 240.0f) {
        r = 0.0f;
        g = x;
        b = c;
      } else if (hue < 300.0f) {
        r = x;
        g = 0.0f;
        b = c;
      } else {
        r = c;
        g = 0.0f;
        b = x;
      }

      // adjust the RGB values for the pastel effect
      r = r * 0.8f + 0.6f;
      g = g * 0.8f + 0.6f;
      b = b * 0.8f + 0.6f;

      // create a Vec4 color from the RGB values and pass it to the mapBlockFunc function
      ngl::Vec4 color = ngl::Vec4(r, g, b, 1.0f);
      mapBlockFunc(mapBlock, countNumBlock, color, white, ngl::Vec3(static_cast<float>(col - (nrColumns / 2)) * spacing, -1, static_cast<float>(row - (nrRows / 2)) * spacing));

      countNumBlock++;
    }
  }

  // start of enemy blocks
  // offset in x direction
  int enemyXOffset = 2;
  // offset in z direction
  int enemyZOffset = -2;
  // speed that enemy blocks move in
  float enemySpeed = 0.09f;

  // moves enemies based on time count
  if (m_timeCount ==  0.1) 
  {
    m_zEnemyLoc++;
  }
  // combining two if statements using the ternary operator ?
  m_xEnemyLoc += (m_timeCount >= 0 && m_timeCount < 50) ? enemySpeed : -enemySpeed;

  // adjust this to change the speed of the pulsing effect
  float frequency = 0.5f;

    for (int i = 0; i < 4; i++) {  // iterate through rows
    for (int j = 0; j < 4; j++) {  // iterate through columns
      // calculate the index of the block in the 1D array
      int index = i * 4 + j;
      // calculate the color of the block based on the pulsing effect
      float pulsation = sin(frequency * m_timeCount);
      // starts from red and fades to black
      ngl::Vec4 blockColor = ngl::Vec4(1.0f - pulsation, 0.0f, 0.0f, 1.0f);
      if (enemyBlocks[index].m_hasBeenKilled == 0) 
      {
        // set the position of the block based on its row and column indices
        enemyBlocks[index].m_position = ngl::Vec3(m_xEnemyLoc + j * 2, 0, m_zEnemyLoc + i * 2);
      } 
      else 
      {
        enemyBlocks[index].m_position = ngl::Vec3(0, -99, 0); 
      }
      mapBlockFunc(mapBlock, index, blockColor, red, enemyBlocks[index].m_position);
    }
  }

  for (int o = 0; o < m_numBulletShot; o++) 
  {
    // checks if block if fired
    if (bulletShot[o].m_hasBeenFired == 1) 
    {
      // updates position of bulletShot if fired
      bulletShot[o].m_position.m_z = bulletShot[o].m_position.m_z - 0.2f;
      mapBlockFunc(bulletShot, o, white, white, ngl::Vec3(bulletShot[o].m_xStartPosition, 0, bulletShot[o].m_position.m_z));
      // check if bulletShot has reached a certain position and delete it if it has
      if (bulletShot[o].m_position.m_z < -10.0f) 
      {
        bulletShot[o].m_position = ngl::Vec3(0.0f, -100.0f, 10.0f);
        bulletShot[o].m_hasBeenFired = 0;
      }
    }
    // else if the bulletShot has not been fired, set it to a position off screen
    else 
    {
     bulletShot[o].m_position = ngl::Vec3(0.0f, -100.0f, 10.0f);
    }
  }



  for (int i = 0; i < 3; i++) 
  {
    ngl::Vec3 m_position = ngl::Vec3(i * 2 - 2, 0, 2); // calculate the position of the block
    mapBlockFunc(mapBlock, countNumBlock + i, orange, black, m_position); // place the block
  }
  

  int CollsionTestTrue;


  for (int u = 0; u < m_numBulletShot; u++)
  {
    for (int t = 0; t < numOfEnemies; t++)
    {
        int ifHit = 0;
        ifHit = checkAABBCollision( bulletShot[u].m_position, bulletShot[u].m_width, bulletShot[u].m_depth, bulletShot[u].m_height, 
                            enemyBlocks[t].m_position, enemyBlocks[t].m_width, enemyBlocks[t].m_depth, enemyBlocks[t].m_height);
        if (ifHit == 1)
        {
          enemyBlocks[t].m_hasBeenKilled = 1.0f;
          // changes position to off screen if hit
          bulletShot[u].m_position = ngl::Vec3(0.0f, -100.0f, 10.0f);
          // if bullet hits enemy it adds 40 to score
          m_score = m_score + 40;
        }
    }
  }
    


  

  // setting key controls
  foreach (Qt::Key key, m_keysPressed)
  {
    switch (key)
    {
    // moves player's character right
    case Qt::Key_Right:
    {
      mySelector.m_displacement = {ngl::Vec3(mySelector.m_displacementAmount,0.0f,0.0f)};
      keyPressed = 1;
      mySelector.moveNextPosition();
      CollisionTest(mapBlock, m_numMap);
      break;
    }
    // moves player's character left
    case Qt::Key_Left:
    {
      mySelector.m_displacement = {ngl::Vec3(-mySelector.m_displacementAmount,0.0f,0.0f)};
      keyPressed = 1;
      mySelector.moveNextPosition();
      CollisionTest(mapBlock, m_numMap);
      break;
    }
    // shoots blocks with spacebar
    case Qt::Key_Space: 
    {
      if (m_keyPressTimer == 0) 
      {
        bulletShot[m_blockNumToShoot].m_xStartPosition = mySelector.m_position.m_x;
        bulletShot[m_blockNumToShoot].m_position.m_z = 10;
        bulletShot[m_blockNumToShoot].m_hasBeenFired = 1;
        std::cout<<"Bullet number "<<m_blockNumToShoot<<"\n";

        if (m_blockNumToShoot < 499) 
          {
            m_blockNumToShoot++;
          } 
          else 
          {
            m_blockNumToShoot = 1;
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
      m_modeToDraw = 1;
      break;
    }
    case Qt::Key_2:
    {
      m_modeToDraw = 2;
      break;
    }
    case Qt::Key_3:
    {
      m_modeToDraw = 3;
      break;
    }

    // block scale controls
    case Qt::Key_4:
    {
      if (mapBlock[1].m_blockScale >= 0) 
      {
        for (int u = 0; u < m_numMap; u++)
        {
          mapBlock[u].m_blockScale = mapBlock[u].m_blockScale - 0.01f;
        }
      }
      break;
    }
    case Qt::Key_5:
    {
      for (int u = 0; u < m_numMap; u++)
      {
        mapBlock[u].m_blockScale = mapBlock[u].m_blockScale + 0.01f;
      }
      break;
    }
    case Qt::Key_6:
    {
      for (int u = 0; u < m_numMap; u++)
      {
        mapBlock[u].m_blockScale = 0.9f;
      }
      break;
    }
    case Qt::Key_7:
    {
      for (int u = 0; u < m_numMap; u++)
      {
        mapBlock[u].m_blockScale = 1.0f;
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
  if (m_modeToDraw == 2)
  {
    m_box->setDrawMode(GL_FILL);
    loadMatricesToShader(lilac);
    m_box->draw();
  } 
  else if (m_modeToDraw == 3)
  {
    m_box->setDrawMode(GL_LINE);
    loadMatricesToShader(black);
    m_box->draw();
  } 
  else 
  {
    m_box->setDrawMode(GL_FILL);
    loadMatricesToShader(lilac);
    m_box->draw();
    m_box->setDrawMode(GL_LINE);
    loadMatricesToShader(black);
    m_box->draw();
  }
  }
  else 
  {
  if (m_modeToDraw == 2)
  {
    m_box->setDrawMode(GL_FILL);
    loadMatricesToShader(pink);
    m_box->draw();
  } 
  else if (m_modeToDraw == 3)
  {
    m_box->setDrawMode(GL_LINE);
    loadMatricesToShader(white);
    m_box->draw();
  } 
  else 
  {
    m_box->setDrawMode(GL_FILL);
    loadMatricesToShader(pink);
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
  painter.drawText(QPoint(12, 40), "score: " + QString::number(m_score));
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

