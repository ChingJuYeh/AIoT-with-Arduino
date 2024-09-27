struct LED : Service::LightBulb {

  int ledPin;
  SpanCharacteristic *power;
  
  LED(int ledPin) : Service::LightBulb(){

    power=new Characteristic::On();
    this->ledPin=ledPin;
    pinMode(ledPin,OUTPUT);    
    //WEBLOG("Configuring LED on Pin %d",ledPin);                         // NEW!  This creates a Web Log message announcing the configuration of the device
    
  } // end constructor

  boolean update(){            

    digitalWrite(ledPin,power->getNewVal());    
    //WEBLOG("LED on Pin %d: %s",ledPin,power->getNewVal()?"ON":"OFF");   // NEW!  This creates a Web Log message whenever an LED is turned ON or OFF
    return(true);
  
  } // update
};