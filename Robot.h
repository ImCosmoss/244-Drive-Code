// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

//calling libarys
#include <frc/Joystick.h>
#include <frc/TimedRobot.h>
#include <frc/drive/DifferentialDrive.h>
#include <frc/motorcontrol/PWMSparkMax.h>
#include <frc/motorcontrol/MotorControllerGroup.h>
#include <frc/XboxController.h>
#include <frc/Compressor.h>
#include <frc/Solenoid.h>
#include <frc/Timer.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <cameraserver/CameraServer.h>
#include <frc/Timer.h>
#include <frc/PS4Controller.h>
/**
 * This is a demo program showing the use of the DifferentialDrive class.
 * Runs the motors with tank steering.
 */
//definitions 
class Robot : public frc::TimedRobot {
  frc::PWMSparkMax m_frontLeftMotor{2};
  frc::PWMSparkMax m_frontRightMotor{4};
  frc::PWMSparkMax m_backLeftMotor{3};
  frc::PWMSparkMax m_backRightMotor{5};
  frc::MotorControllerGroup m_left{m_frontLeftMotor, m_backLeftMotor}; 
  frc::MotorControllerGroup m_right{m_frontRightMotor, m_backRightMotor};
  frc::DifferentialDrive m_robotDrive{m_left, m_right};
  // frc::XboxController controller{0};
  frc::PWMSparkMax m_hookaxel{0};
  frc::XboxController controller2{1};
  frc::Timer timer1;
  frc::PS4Controller PController{0};
  
  //winch and axel motor callings\/
  frc::PWMSparkMax m_winch1{7}; 
  frc::PWMSparkMax m_winch2{8};
  frc::MotorControllerGroup winchgroup{m_winch1, m_winch2}; //motor group for tanner because he like dem as group
 

  //compressor code and solenoid code\/
  
  frc::Compressor compressor1{1, frc::PneumaticsModuleType::REVPH}; //0 is port and CREPCM is the board
  frc::Solenoid solenoid1{frc::PneumaticsModuleType::REVPH, 6}; //your gay
  frc::Solenoid solenoid2{frc::PneumaticsModuleType::REVPH, 8};//0 is the port number and CREPCM is the board

  //uptake+intake and intake controller calling

  frc::PWMSparkMax m_intakecontroller{9}; //1 was the port previously
  frc::PWMSparkMax m_intake{1}; //9 was the port previously
  frc::PWMSparkMax m_uptake1{6};
 
 public:
 // moves robot forward, takes a parameter for motor power, and an integer for speed

 //activates solenoids to shoot arm
 void shoot() {
   solenoid1.Set(false);
   solenoid2.Set(false);
 }

 // switches solenoids to false
 
 void retract() {
   solenoid1.Set(true);
   solenoid2.Set(true);
 }
 
 void moveupintake(double speed, int times,int sleepup) { //move the robot forward(negitive) and backwards(postitive)
   m_left.Set(speed);
   m_right.Set(speed);
   m_intake.Set(-1);
   m_uptake1.Set(-1);
   sleep(times);
   m_left.StopMotor();
   m_right.StopMotor();
   sleep(sleepup);
   m_intake.StopMotor();
   m_uptake1.StopMotor();
 }
 void intakemove (double speed,int times,bool turnto) {//make sure the intake speed on the paramter is 1, so that the 1 for the sleep in the code and the 1 in the parameter = 2 seconds
     if(turnto == false) {
       m_intakecontroller.Set(speed);
       sleep(1);
       m_intake.Set(-1);
       sleep(times);
       m_intakecontroller.StopMotor();
     } else if(turnto == true) {
       m_intakecontroller.Set(-speed);
       sleep(1);
       m_intake.Set(-1);
       sleep(times);
       m_intakecontroller.StopMotor();
     }
   }
 void turn(double speed, int times,bool turnvalue) { //theres 3 parameters the last one is either a true or false(bool) if its true the robot turns left. if set to false it turns right
   if(turnvalue == true) {
     timer1.Reset();
     m_left.Set(speed);//turn lef
     //makes the next number negitive
     m_right.Set(-speed);
     sleep(times);
     m_left.StopMotor();
     m_right.StopMotor();
   } else if(turnvalue == false) {
     m_right.Set(speed);//turn right
     //makes the next number negitive
     m_left.Set(-speed);
     sleep(times);
     m_left.StopMotor();
     m_right.StopMotor();
   }
 }
   void turn1(double times, double speed) {
     timer1.Reset();
     while((double)timer1.Get() < times)
     {
       m_right.Set(speed);
       m_left.Set(-speed);
     }
       m_right.Set(0);
       m_left.Set(0);
 } void move(double speed, int times) {
   m_left.Set(speed);
   m_right.Set(speed);
   sleep(times);
   m_left.StopMotor();
   m_right.StopMotor();
 }
 void intakemovedown(double speed,int times) {
   m_intakecontroller.Set(speed);
   sleep(times);
   m_intakecontroller.StopMotor();
 }
 void uptake(double speed, int times) {
   m_uptake1.Set(-speed);
   sleep(times);
   m_uptake1.StopMotor();
 }

  void RobotInit() override {
    // We need to invert one side of the drivetrain so that positive voltages
    // result in both sides moving forward. Depending on how your robot's
    // gearbox is constructed, you might have to invert the left side instead.
    m_left.SetInverted(true);
    //compressor run
    compressor1.Start();

    frc::CameraServer::GetInstance()->StartAutomaticCapture(); //call Camera object
    frc::CameraServer::GetInstance()->StartAutomaticCapture(1);
    m_frontRightMotor.SetSafetyEnabled(false);
    m_frontLeftMotor.SetSafetyEnabled(false);
    m_backLeftMotor.SetSafetyEnabled(false);
    m_backRightMotor.SetSafetyEnabled(false);
    m_robotDrive.SetSafetyEnabled(false);
  }

  void AutonomousInit() override {
    compressor1.Start();
    timer1.Start();
    //auton 2 ball yes
    
    //*
    intakemove(0.6, 1,false);//intake moves down
    moveupintake(-.5,1,1.7); // move forward with intake going(speed, sleep, intakewait) 
    sleep(1);
    turn1(1.5,.63); // turn to shoot|(1.1,.62) values accurate 3/29/22, turn value for 2 ball(1.1)
    move(-.4,1); // move forward to shoot 
    sleep(1);
    retract();// first shot
    sleep(1);
    shoot(); //retract
    sleep(1);
    uptake(1,2); // bring second ball into shooter
    retract(); // second shot
    sleep(1);
    shoot();// final retract
    //*/

    //end of auton 2 ball yes

    //auton 1 ball yes
    
    /*
    
    move(.4,1);
    sleep(6);
    retract(); //flipped this shoots
    sleep(1);
    shoot(); //flipped this retracts
    move(.4,1);
    intakemovedown(0.6,2);
    
    */
    //end of auton 1 ball yes
  }
  void TeleopInit() override {

  }
  
  void TeleopPeriodic() override {
    // Drive with tank style
    m_robotDrive.TankDrive(PController.GetLeftY(), PController.GetRightY());
    //driver code
    
    if(controller2.GetAButtonPressed()) {
      m_hookaxel.Set(.65);
    }
    if(controller2.GetBButtonPressed()) {
      m_hookaxel.Set(-.35);
    }
    if(controller2.GetAButtonReleased() || controller2.GetBButtonReleased()){
      m_hookaxel.StopMotor();
    }
    if (PController.GetL1ButtonPressed()) {
      m_intake.Set(-.9);
    }
    if(PController.GetL1ButtonReleased()) {
      m_intake.StopMotor();
    }
     if (PController.GetCrossButtonPressed()) {
      m_intake.Set(.9);
    }
    if(PController.GetCrossButtonReleased()) {
      m_intake.StopMotor();
    }
    if (controller2.GetBackButtonPressed()) {//intake all systems up
      m_uptake1.Set(1);

    }
    if (controller2.GetBackButtonReleased()) {
      m_uptake1.StopMotor();
    }
    if(controller2.GetStartButtonPressed()) { //intake all systems down
      m_uptake1.Set(-1);
    } 
    if(controller2.GetStartButtonReleased()) {
      m_uptake1.StopMotor();
    }
    if (PController.GetR1ButtonPressed()) { //sets the lacroso shooter back to retarcted position (GetAButtonPressed)
      solenoid1.Set(true);
      solenoid2.Set(true);
    } 
    if (PController.GetR2Axis()>0) { //fires the lacroso shooter
      solenoid1.Set(false);
      solenoid2.Set(false);

    } 
    if(controller2.GetXButtonPressed()) {
      m_winch1.Set(1);
      m_winch2.Set(1);
    }
    if(controller2.GetXButtonReleased()) {
      m_winch2.StopMotor();
      m_winch1.StopMotor();
    }
    if(controller2.GetYButtonPressed()) {
      m_winch1.Set(-1);
      m_winch2.Set(-1);
    }
    if(controller2.GetYButtonReleased()) {
      m_winch2.StopMotor();
      m_winch1.StopMotor();
    }
    if(controller2.GetRightBumperPressed()) {
      m_intakecontroller.Set(.8);
    }
    if(controller2.GetRightBumperReleased() || controller2.GetLeftBumperReleased()){
      m_intakecontroller.StopMotor();
    }
    if(controller2.GetLeftBumperPressed()){
      m_intakecontroller.Set(-.6);
    }
    
    // else{
    //   solenoid1.Set(false);
    // }
  }
};

#ifndef RUNNING_FRC_TESTS
int main() {
  return frc::StartRobot<Robot>();
}
#endif
