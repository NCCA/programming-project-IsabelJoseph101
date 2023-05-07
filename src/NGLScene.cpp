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

  std::generate(std::begin(m_BlockSelectorArray), std::end(m_BlockSelectorArray), [this]()
                { return BlockSelector();});
}

int NGLScene::CollisionTest()
{
  
  mySelector.moveNextPosition();

  int checkResult = 0;

  float mySelectormMinX = mySelector.nextPosition.m_x - mySelector.blockScale/2.0f; 
  float mySelectormMaxX = mySelector.nextPosition.m_x + mySelector.blockScale/2.0f; 
  float mySelectormMinY = mySelector.nextPosition.m_y - mySelector.blockScale/2.0f; 
  float mySelectormMaxY = mySelector.nextPosition.m_y + mySelector.blockScale/2.0f; 
  float mySelectormMinZ = mySelector.nextPosition.m_z - mySelector.blockScale/2.0f; 
  float mySelectormMaxZ = mySelector.nextPosition.m_z + mySelector.blockScale/2.0f; 
  



  float mapBoxMinX = mapBox.position.m_x - mapBox.blockScale/2.0f; 
  float mapBoxMaxX = mapBox.position.m_x + mapBox.blockScale/2.0f; 
  float mapBoxMinY = mapBox.position.m_y - mapBox.blockScale/2.0f; 
  float mapBoxMaxY = mapBox.position.m_y + mapBox.blockScale/2.0f; 
  float mapBoxMinZ = mapBox.position.m_z - mapBox.blockScale/2.0f; 
  float mapBoxMaxZ = mapBox.position.m_z + mapBox.blockScale/2.0f; 

  


  // if (mySelectormMinX  > mapBoxMaxX) {
  //   checkResult = 1;
  // }
  // if (mySelectormMaxX  < mapBoxMinX) {
  //   checkResult = 1;
  // }
  // if (mySelectormMinY  > mapBoxMaxY) {
  //   checkResult = 1;
  // }
  // if (mySelectormMaxY  < mapBoxMinY) {
  //   checkResult = 1;
  // }

  // if (mySelectormMinZ  > mapBoxMaxZ) {
  //   checkResult = 1;
  // }
  // if (mySelectormMaxZ  < mapBoxMinZ) {
  //   checkResult = 1;
  // }


  if (mySelector.nextPosition == mapBox.position) {
    checkResult = 1;
  }
  else 
  {
    checkResult = 0;
  }

  std::cout<<"Check Result = "<<checkResult<<"\n";
  std::cout<<"Map BoX Center =     "<<mapBox.position<<"\n"; 
  std::cout<<"My Selector Center = "<<mySelector.nextPosition<<"\n"; 

  if (checkResult == 1) {
    std::cout<<"collisions detected -----------------------------------------------------------------------------------------------------------------------"<<"\n";
    // mySelector.move();
  }
  else {
    std::cout<<"collisions not detected"<<"\n";
    // mySelector.move();
    
    
  }

  return checkResult;

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

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // White Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
// enable multisampling for smoother drawing
#ifndef USINGIOS_
  glEnable(GL_MULTISAMPLE);
#endif
  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from(0, 5, 20);
  ngl::Vec3 to(0, 0, 0);
  ngl::Vec3 up(0, 1, 0);
  // now load to our new camera
  m_cam.set(from, to, up);
  // set the shape using FOV 90 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam.setProjection(90.0f, 720.0f / 576.0f, 0.05f, 350.0f);

  ngl::ShaderLib::use(ngl::nglColourShader);
  ngl::ShaderLib::setUniform("Colour", 1.0f, 1.0f, 1.0f, 1.0f);
  // ngl::VAOPrimitives::createTrianglePlane("floor", 650, 650, 1, 1, ngl::Vec3::up());
  ngl::VAOPrimitives::createLineGrid("plane", 24, 24, 24);
  // ngl::VAOPrimitives::createLineGrid("planeWall", 20, 20, 20);

  m_bbox = std::make_unique<ngl::BBox>(ngl::Vec3(0.0f, 0.0f, 0.0f), mapBox.blockScale, mapBox.blockScale, mapBox.blockScale);
  m_bbox2 = std::make_unique<ngl::BBox>(ngl::Vec3(0.0f, 0.0f, 0.0f), mySelector.blockScale, mySelector.blockScale, mySelector.blockScale);
  mySelector.position = {ngl::Vec3(0.0f,1.0f,0.0f)};
  
  

  // m_bboxBounding = std::make_unique<ngl::BBox>(ngl::Vec3(0.0f, 0.0f, 0.0f), 26.0f, 26.0f, 1.0f);


}

void NGLScene::loadMatricesToShader(ngl::Vec4 &_colour)
{
  ngl::Mat4 MVP = m_cam.getVP() * m_transform.getMatrix();

  ngl::ShaderLib::setUniform("MVP", MVP);
  ngl::ShaderLib::setUniform("Colour", _colour);


//   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//   ngl::Vec4 red(1.0f, 0.0f, 0.0f, 1.0f);
//   loadMatricesToShader(red);
//   m_transform.reset();
//   m_transform.setPosition(6.0f, 6.0f, 6.0f);
//   ngl::VAOPrimitives::draw("SelectedsquareCube");

// }
}


void NGLScene::paintGL()
{
  // BlockSelector::BlockSelector mySelector;
  // BlockSelector=std::make_unique<mySelector>();
  
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
  int keyPressed = 0;

  // mySelector.position.m_y = 1.0f;
 
  // now we loop for each of the pressed keys in the the set
  // and see which ones have been pressed. If they have been pressed
  // we set the movement value to be an incremental value
  // foreach (Qt::Key key, m_keysPressed)
  // {
  //   switch (key)
  //   {
  //   case Qt::Key_Left:
  //   {
  //     yDirection = -5.0f;
  //     break;
  //   }
  //   case Qt::Key_Right:
  //   {
  //     yDirection = 5.0f;
  //     break;
  //   }
  //   case Qt::Key_Up:
  //   {
  //     xDirection = 5.0f;
  //     break;
  //   }
  //   case Qt::Key_Down:
  //   {
  //     xDirection = -5.0f;
  //     break;
  //   }
  //   case Qt::Key_6:
  //   {
  //     // mySelector.position.m_x = mySelector.position.m_x + 1;
  //     mySelector.displacement = {ngl::Vec3(mySelector.displacementAmount,0.0f,0.0f)};
  //     keyPressed = 1;
  //     mySelector.moveNextPosition();
  //     // m_bbox2->setCenter(mySelector.position);

      
  //     break;
  //   }
  //   case Qt::Key_4:
  //   {
  //     // mySelector.position.m_x = mySelector.position.m_x - 1;
  //     mySelector.displacement = {ngl::Vec3(-mySelector.displacementAmount,0.0f,0.0f)};
  //     keyPressed = 1;
  //     mySelector.moveNextPosition();
  //     // m_bbox2->setCenter(mySelector.position);

      
  //     break;
  //   }
  //   case Qt::Key_E:
  //   {
  //     // mySelector.position.m_x = mySelector.position.m_y + 1;
  //     mySelector.displacement = {ngl::Vec3(0.0f,mySelector.displacementAmount,0.0f)};
  //     keyPressed = 1;
  //     mySelector.moveNextPosition();
      
  //     // m_bbox2->setCenter(mySelector.position);   

        
  //     break;
  //   }
  //   case Qt::Key_D:
  //   {
  //     // if (mySelector.position.m_y > 0){
  //     // mySelector.position.m_x = mySelector.position.m_y - 1;
  //     mySelector.displacement = {ngl::Vec3(0.0f,-mySelector.displacementAmount,0.0f)};
  //     keyPressed = 1;
  //     mySelector.moveNextPosition();
  //     // mySelector.move();
  //     // mySelector.nextPosition = mySelector.position + mySelector.displacement;

  //     // m_bbox2->setCenter(mySelector.position);
     
  //     // }
  //     break;
  //   }
  //   case Qt::Key_5:
  //   {
  //     // mySelector.position.m_x = mySelector.position.m_x + 1;
  //     mySelector.displacement = {ngl::Vec3(0.0f,0.0f,mySelector.displacementAmount)};
  //     keyPressed = 1;
  //     mySelector.moveNextPosition();
  //     // m_bbox2->setCenter(mySelector.position);
 
  
  //     break;
  //   }
  //   case Qt::Key_8:
  //   {
      
  //     // mySelector.position.m_x = mySelector.position.m_x - 1;
  //     mySelector.displacement = {ngl::Vec3(0.0f,0.0f,-mySelector.displacementAmount)};
  //     keyPressed = 1;
  //     mySelector.moveNextPosition();
  //     // m_bbox2->setCenter(mySelector.position);
    
      

  //     break;
  //   }
  //   default:
  //     break;
  //   }
  // }
  // if the set is non zero size we can update the ship movement
  // then tell openGL to re-draw
  if (m_keysPressed.size() != 0)
  {
    m_cam.move(xDirection, yDirection, m_deltaTime);
  }


  // std::cout << "mySelector Position = " << mySelector.position << '\n';

  

  int nrRows = 20;
  int nrColumns = 20;
  float offsetSpacing = 0.5;
  float spacing = 1.0;
  ngl::Random::setSeed(1234);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  ngl::Vec4 white(1.0f, 1.0f, 1.0f, 1.0f);
  ngl::Vec4 red(1.0f, 0.0f, 0.0f, 1.0f);
  ngl::Vec4 green(0.0f, 0.5f, 0.0f, 1.0f);
  ngl::Vec4 yellow(1.0f, 1.0f, 0.0f, 1.0f);
  ngl::Vec4 blue(0.0f, 0.0f, 1.0f, 1.0f);
  ngl::Vec4 black(0.0f, 0.0f, 0.0f, 1.0f);
  ngl::Vec4 darkGray(0.2f, 0.2f, 0.2f, 1.0f);
  ngl::Vec4 redSelect(0.8f, 0.0f, 0.0f, 1.0f);
  // int colourLoop = 2;

  int CollsionTestTrue;
  
  for (int row = 0; row < nrRows; ++row)
  {
    for (int col = 0; col < nrColumns; ++col)
    {
      
      // if (colourLoop < 3) {
      //   colourLoop = colourLoop + 1;
      // } 
      // else {
      //   colourLoop = 1;
      // } 
      
      // colourLoop = (ngl::Random::randomPositiveNumber() * 3.0f + 1);

      // m_bbox->setCenter(ngl::Vec3(0.0f,0.0f,mySelector.displacementAmount)});
      

      // m_transform.setPosition(static_cast<float>(col - (nrColumns / 2)) * spacing,
      //                         -1.0f,
      //                         static_cast<float>(row - (nrRows / 2)) * spacing);
      // // m_transform.setRotation(0.0f, ngl::Random::randomPositiveNumber() * 360.0f, 0.0f);
      //  m_transform.setScale(mapBox.blockScale, mapBox.blockScale, mapBox.blockScale);
    // CollsionTestTrue = 0;
      
    foreach (Qt::Key key, m_keysPressed)
  {
    switch (key)
    {
    case Qt::Key_Left:
    {
      yDirection = -5.0f;
      break;
    }
    case Qt::Key_Right:
    { 
      yDirection = 5.0f;
      break;
    }
    case Qt::Key_Up:
    {
      xDirection = 5.0f;
      break;
    }
    case Qt::Key_Down:
    {
      xDirection = -5.0f;
      break;
    }
    case Qt::Key_6:
    {
      // mySelector.position.m_x = mySelector.position.m_x + 1;
      mySelector.displacement = {ngl::Vec3(mySelector.displacementAmount,0.0f,0.0f)};
      keyPressed = 1;
      mySelector.moveNextPosition();
      // m_bbox2->setCenter(mySelector.position);

      
      break;
    }
    case Qt::Key_4:
    {
      // mySelector.position.m_x = mySelector.position.m_x - 1;
      mySelector.displacement = {ngl::Vec3(-mySelector.displacementAmount,0.0f,0.0f)};
      keyPressed = 1;
      mySelector.moveNextPosition();
      // m_bbox2->setCenter(mySelector.position);

      
      break;
    }
    case Qt::Key_E:
    {
      // mySelector.position.m_x = mySelector.position.m_y + 1;
      mySelector.displacement = {ngl::Vec3(0.0f,mySelector.displacementAmount,0.0f)};
      keyPressed = 1;
      mySelector.moveNextPosition();
      // std::cout<<"Displacement = "<<mySelector.displacement<<"\n";
      // std::cout<<"Next Position = "<<mySelector.nextPosition<<"\n";
      // std::cout<<"Position = "<<mySelector.position<<"\n";
      // m_bbox2->setCenter(mySelector.position);   

        
      break;
    }
    case Qt::Key_D:
    {
      // if (mySelector.position.m_y > 0){
      // mySelector.position.m_x = mySelector.position.m_y - 1;
      mySelector.displacement = {ngl::Vec3(0.0f,-mySelector.displacementAmount,0.0f)};
      keyPressed = 1;
      mySelector.moveNextPosition();
      // mySelector.move();
      // mySelector.nextPosition = mySelector.position + mySelector.displacement;
      // std::cout<<"Displacement = "<<mySelector.displacement<<"\n";
      // std::cout<<"Next Position = "<<mySelector.nextPosition<<"\n";
      // std::cout<<"Position = "<<mySelector.position<<"\n";
      // m_bbox2->setCenter(mySelector.position);
     
      // }
      break;
    }
    case Qt::Key_5:
    {
      // mySelector.position.m_x = mySelector.position.m_x + 1;
      mySelector.displacement = {ngl::Vec3(0.0f,0.0f,mySelector.displacementAmount)};
      keyPressed = 1;
      mySelector.moveNextPosition();
      // m_bbox2->setCenter(mySelector.position);
 
  
      break;
    }
    case Qt::Key_8:
    {
      
      // mySelector.position.m_x = mySelector.position.m_x - 1;
      mySelector.displacement = {ngl::Vec3(0.0f,0.0f,-mySelector.displacementAmount)};
      keyPressed = 1;
      mySelector.moveNextPosition();
      // m_bbox2->setCenter(mySelector.position);
    
      

      break;
    }
    default:
      break;
    }
    update();
  }
    if (m_keysPressed.size() != 0)
  {
    m_cam.move(xDirection, yDirection, m_deltaTime);
  }

      m_bbox->setCenter(ngl::Vec3(static_cast<float>(col - (nrColumns / 2)) * spacing,
                                  -1.0f,
                                  static_cast<float>(row - (nrRows / 2)) * spacing));
      
      mapBox.position = (ngl::Vec3(static_cast<float>(col - (nrColumns / 2)) * spacing,
                                  -1.0f,
                                  static_cast<float>(row - (nrRows / 2)) * spacing));

      

      if (keyPressed == 1) {
        // std::cout<<" KeyPressed got here"<<"\n";
        if (CollsionTestTrue == 1) {
          std::cout<<"hit"<<"\n";
        }
        else {
          CollsionTestTrue = CollisionTest();
        }
        std::cout<<"CollsionTestTrue =  "<<CollsionTestTrue<<"\n";
        // std::cout<<"Displacement =  "<<mySelector.displacement<<"\n";
        // std::cout<<"Next Position = "<<mySelector.nextPosition<<"\n";
        // std::cout<<"Position =      "<<mySelector.position<<"\n";
      }

      keyPressed = 0;

      // std::cout << "colourLoop = " << colourLoop << '\n';


      // if (colourLoop == 1) {
      //   loadMatricesToShader(blue);
      // }
      // else if (colourLoop == 2 ) {
      //   loadMatricesToShader(darkGray);
      // }
      //  else if (colourLoop == 3 ) {
      //   loadMatricesToShader(red);
      // }
      

 
      // if (currentFrameClamp % 25 = 0) {
      // loadMatricesToShader(darkGray);
   

      // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      // ngl::VAOPrimitives::draw("cube");

      m_bbox->setDrawMode(GL_FILL);
      loadMatricesToShader(darkGray);
      m_bbox->draw();
      m_bbox->setDrawMode(GL_LINE);
      loadMatricesToShader(white);
      m_bbox->draw();


      // std::cout<<"Block Selector Center: "<<m_bbox2->center()<<"\n";

      // std::cout<<"Block Selector Bounds: "<<m_bbox->minX()<<" "<<m_bbox->maxX()<<" "<<m_bbox->minY()<<" "<<m_bbox->maxY()<<" "<<m_bbox->minZ()<<" "<<m_bbox->maxZ()<<"\n";
      // std::cout<<"Map Block      Bounds: "<<m_bbox2->minX()<<" "<<m_bbox2->maxX()<<" "<<m_bbox2->minY()<<" "<<m_bbox2->maxY()<<" "<<m_bbox2->minZ()<<" "<<m_bbox2->maxZ()<<"\n";

    
      


    }
  }
  // draw floor


  int cccount = 0;
  if (CollsionTestTrue == 0) {
      // std::cout<<" KeyPressed"<<"\n";
      mySelector.move();
      m_bbox2->setCenter(mySelector.position);
      cccount = cccount + 1;
      //std::cout<<"cccount"<<cccount"\n";
    }
  CollsionTestTrue = 1;


  // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  // m_transform.reset();
  // m_transform.setPosition(offsetSpacing, 0, offsetSpacing);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // auto grey = ngl::Vec4(0.2f, 0.2f, 0.2f, 1.0f);
  // loadMatricesToShader(grey);
  // ngl::VAOPrimitives::draw("plane");
  // ngl::VAOPrimitives::draw("floor");


  
      // loadMatricesToShader(grey);



  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  m_transform.reset();
  // m_transform.setRotation(15.0f,0.0f,0.0f);
  // m_transform.setScale(mySelector.blockScale, mySelector.blockScale, mySelector.blockScale);
  // m_transform.setPosition( mySelector.position.m_x, mySelector.position.m_y - (mySelector.blockScale/2), mySelector.position.m_z);


  // loadMatricesToShader(redSelect);
  // m_transform.setScale(mySelector.blockScale, mySelector.blockScale, mySelector.blockScale);


    int currentFrameClamp = static_cast<int>(currentFrame*10);
    int FrameRim = currentFrameClamp % 10;
    // int currentFrameClamp = std::clamp(static_cast<int>(currentFrame), 0, 1000000);
    // std::cout << "Current Frame Clamped = " << currentFrameClamp<< '\n';
    // std::cout << "Current Frame = " << currentFrame<< '\n';
    // std::cout << "Current Frame Reminder = " << FrameRim<< '\n';

    if (FrameRim < 2) {
    
      m_bbox2->setDrawMode(GL_FILL);
      loadMatricesToShader(yellow);
      m_bbox2->draw();
      m_bbox2->setDrawMode(GL_LINE);
      loadMatricesToShader(black);
      m_bbox2->draw();
    }
    else {
      // m_transform.setScale(mySelector.blockScale, mySelector.blockScale, mySelector.blockScale);
      // m_transform.setPosition( mySelector.position.m_x, mySelector.position.m_y - (mySelector.blockScale/2), mySelector.position.m_z);
      m_bbox2->setDrawMode(GL_FILL);
      loadMatricesToShader(red);
      m_bbox2->draw();
      m_bbox2->setDrawMode(GL_LINE);
      loadMatricesToShader(white);
      m_bbox2->draw();


      CollsionTestTrue = 0;

      


      // std::cout<<"X Pos: "<<mySelector.position.m_x<<"\n";
      // mySelector.minXLoc = mySelector.position.m_x - mySelector.blockScale/2;
      // mySelector.maxXLoc = mySelector.position.m_x + mySelector.blockScale/2;
      // mySelector.minYLoc = mySelector.position.m_y - mySelector.blockScale/2;
      // mySelector.maxYLoc = mySelector.position.m_y + mySelector.blockScale/2;
      // mySelector.minZLoc = mySelector.position.m_z - mySelector.blockScale/2;
      // mySelector.maxZLoc = mySelector.position.m_z + mySelector.blockScale/2;
    

      // std::cout<<"Bounds: "<<mySelector.minXLoc<<" "<<mySelector.maxXLoc<<" "<<mySelector.minYLoc<<" "<<mySelector.maxYLoc<<" "<<mySelector.minZLoc<<" "<<mySelector.maxZLoc<<"\n";

      // std::cout<<"minX : "<<minXLoc<<"\n";

    }

  
  
 
  m_transform.reset();



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
  case Qt::Key_N:
    showNormal();
    break;
  case Qt::Key_Space:
    m_win.spinXFace = 0;
    m_win.spinYFace = 0;
    m_modelPos.set(ngl::Vec3::zero());
    break;
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
