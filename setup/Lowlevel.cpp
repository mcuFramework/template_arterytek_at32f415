/**
 * Copyright (c) 2020 ZxyKira
 * All rights reserved.
 * 
 * SPDX-License-Identifier: MIT
 */

/* ****************************************************************************************
 * Include
 */  

//-----------------------------------------------------------------------------------------
#include "mcuf/package-info.h"
#include "core_arterytek_at32f415/package-info.h"
#include "os_cmsis_rtos/package-info.h"
//-----------------------------------------------------------------------------------------

/* ****************************************************************************************
 * Using
 */  

//-----------------------------------------------------------------------------------------
using namespace mcuf;
using namespace mcuf::hal;
using namespace core;

//-----------------------------------------------------------------------------------------

/* ****************************************************************************************
 * Extern
 */
extern "C" void core_at32f415_interrupt_priority(void);

/* ****************************************************************************************
 * Variable
 */
static hal::SerialPort* systemConsoleSerialPort;
static mcuf::Console* systemConsole;
  
/* ****************************************************************************************
 * Method
 */



/**
 * @brief 
 * 
 * @param address 
 * @param code 
 * @return true 
 * @return false 
 */
bool errorCodeHandler(const void* address, ErrorCode code){

  return true;
}

/**
 * @brief 
 * 
 */
void lowlevel_basicInit(void){
  Core::setSystemCoreClock(144);
  
  System::initialize();
  System::getRegister().setInterfaceKernel(new cmsisrtos::CmsisRtosKernel());
  System::getRegister().setInterfaceThread(new cmsisrtos::CmsisRtosThread());
  System::getRegister().setInterfaceTimer(new cmsisrtos::CmsisRtosTimer());
  System::setup();
  
  Core::iomux.init();
  Core::gpioa.init();
  Core::gpiob.init();
  Core::gpioc.init();
  Core::gpiod.init();
  Core::gpiof.init();
  Core::iomux.remapSWDIO(CoreIomux::MapSWDIO::JTAGDISABLE);
}

/**
 * @brief 
 * 
 */
void lowlevel_console(void){
  
  Core::gpioc.setFunction(12, false);
  systemConsoleSerialPort = new CoreSerialPort(CoreSerialPortReg::REG_UART5, 32);
  systemConsoleSerialPort->init();
  systemConsoleSerialPort->baudrate(128000);
  
  systemConsole = new mcuf::ConsoleSerialPort(*systemConsoleSerialPort, 64, 512);
  System::getRegister().setPrintStream(&systemConsole->out());
  System::getRegister().setInputStreamBuffer(&systemConsole->in());
  System::getRegister().setErrorCodeHandler(errorCodeHandler);
  
}


/**
 * @brief 
 * 
 */
void lowlevel(void){
  lowlevel_basicInit();
  lowlevel_console();
  core_at32f415_interrupt_priority();
}  

 
/* ****************************************************************************************
 * End of file
 */ 
