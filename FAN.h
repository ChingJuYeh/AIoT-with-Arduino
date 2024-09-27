struct FAN : Service::LightBulb {

  int fanPin;
  SpanCharacteristic *power;
  
  FAN(int fanPin) : Service::LightBulb(){

    power=new Characteristic::On();
    this->fanPin=fanPin;
    pinMode(fanPin,OUTPUT);    
    //WEBLOG("Configuring FAN on Pin %d",fanPin);                         // NEW!  This creates a Web Log message announcing the configuration of the device
    
  } // end constructor

  boolean update(){            

    digitalWrite(fanPin,power->getNewVal());    
    //WEBLOG("FAN on Pin %d: %s",fanPin,power->getNewVal()?"ON":"OFF");   // NEW!  This creates a Web Log message whenever an FAN is turned ON or OFF
    return(true);
  
  } // update
};