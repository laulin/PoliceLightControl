/*
This code manages a flashing lights a police car. Lights drived :

- red/blue lights behinnd the bumper
- white light near the turn light signal
- TODO ON/OFF flashing light behind the windshield

Each functions have it own button. Two button durations are available, typically :

- a short one (> 100ms) for enabling/disabling
- a long one (> 500 ms) for the mode changing

An additionnal button is available as emergency shutdown (short push) / enabling (long push).

*/

#include <EEPROM.h>

// this time in ms gives a loop cycle duration
#define LOOP_DELAY_MS (25)

// number of loop needed for push button
#define BUTTON_SHORT_PUSH_TIME 100 /* ms */
#define BUTTON_SHORT_PUSH (BUTTON_SHORT_PUSH_TIME / LOOP_DELAY_MS)
#define BUTTON_LONG_PUSH_TIME 500 /* ms */
#define BUTTON_LONG_PUSH (BUTTON_LONG_PUSH_TIME / LOOP_DELAY_MS)  

// masks used by patterns for dual lights
#define RED_MASK 1
#define BLUE_MASK 2
#define NONE_MASK 0

// patterns to display in dual lights
#define DUAL_FLASHING_LIGHT_MAX_MODE 5

struct pattern_stream{
  byte pattern_size;
  byte pattern_value[256];
};

struct pattern_stream DUAL_FLASH_PATTERNS[DUAL_FLASHING_LIGHT_MAX_MODE] = {
  {
   255,
     {
      RED_MASK|BLUE_MASK,BLUE_MASK,NONE_MASK,NONE_MASK,BLUE_MASK,BLUE_MASK,NONE_MASK,RED_MASK|BLUE_MASK,
      RED_MASK,RED_MASK|BLUE_MASK,RED_MASK,RED_MASK,BLUE_MASK,BLUE_MASK,RED_MASK,RED_MASK|BLUE_MASK,
      RED_MASK,RED_MASK|BLUE_MASK,RED_MASK|BLUE_MASK,RED_MASK,RED_MASK,BLUE_MASK,RED_MASK,RED_MASK,
      NONE_MASK,RED_MASK,NONE_MASK,BLUE_MASK,RED_MASK,BLUE_MASK,RED_MASK|BLUE_MASK,RED_MASK,
      RED_MASK,RED_MASK|BLUE_MASK,NONE_MASK,BLUE_MASK,RED_MASK,RED_MASK|BLUE_MASK,NONE_MASK,NONE_MASK,
      NONE_MASK,RED_MASK,RED_MASK|BLUE_MASK,RED_MASK,NONE_MASK,RED_MASK|BLUE_MASK,RED_MASK,RED_MASK|BLUE_MASK,
      RED_MASK|BLUE_MASK,BLUE_MASK,RED_MASK,BLUE_MASK,RED_MASK,NONE_MASK,BLUE_MASK,NONE_MASK,
      RED_MASK,BLUE_MASK,BLUE_MASK,BLUE_MASK,BLUE_MASK,RED_MASK|BLUE_MASK,BLUE_MASK,RED_MASK,
      NONE_MASK,BLUE_MASK,NONE_MASK,BLUE_MASK,BLUE_MASK,RED_MASK,RED_MASK,NONE_MASK,
      RED_MASK|BLUE_MASK,RED_MASK|BLUE_MASK,NONE_MASK,RED_MASK|BLUE_MASK,BLUE_MASK,NONE_MASK,RED_MASK,NONE_MASK,
      RED_MASK|BLUE_MASK,BLUE_MASK,RED_MASK,BLUE_MASK,BLUE_MASK,BLUE_MASK,BLUE_MASK,RED_MASK,
      RED_MASK,NONE_MASK,NONE_MASK,NONE_MASK,BLUE_MASK,NONE_MASK,RED_MASK,BLUE_MASK,
      NONE_MASK,BLUE_MASK,BLUE_MASK,RED_MASK|BLUE_MASK,RED_MASK|BLUE_MASK,RED_MASK,NONE_MASK,NONE_MASK,
      BLUE_MASK,BLUE_MASK,BLUE_MASK,RED_MASK,NONE_MASK,BLUE_MASK,RED_MASK|BLUE_MASK,RED_MASK|BLUE_MASK,
      RED_MASK|BLUE_MASK,NONE_MASK,RED_MASK,NONE_MASK,BLUE_MASK,RED_MASK,BLUE_MASK,NONE_MASK,
      BLUE_MASK,RED_MASK,BLUE_MASK,RED_MASK|BLUE_MASK,RED_MASK|BLUE_MASK,NONE_MASK,RED_MASK,BLUE_MASK,
      RED_MASK|BLUE_MASK,NONE_MASK,NONE_MASK,RED_MASK,RED_MASK|BLUE_MASK,RED_MASK,RED_MASK,RED_MASK,
      RED_MASK|BLUE_MASK,NONE_MASK,NONE_MASK,RED_MASK,RED_MASK|BLUE_MASK,RED_MASK,RED_MASK,NONE_MASK,
      RED_MASK|BLUE_MASK,BLUE_MASK,RED_MASK,RED_MASK|BLUE_MASK,RED_MASK,BLUE_MASK,BLUE_MASK,BLUE_MASK,
      RED_MASK,RED_MASK,BLUE_MASK,RED_MASK,NONE_MASK,RED_MASK,RED_MASK,NONE_MASK,
      NONE_MASK,RED_MASK|BLUE_MASK,NONE_MASK,RED_MASK|BLUE_MASK,RED_MASK|BLUE_MASK,RED_MASK,NONE_MASK,NONE_MASK,
      RED_MASK|BLUE_MASK,NONE_MASK,NONE_MASK,NONE_MASK,BLUE_MASK,BLUE_MASK,RED_MASK,NONE_MASK,
      RED_MASK|BLUE_MASK,BLUE_MASK,RED_MASK,BLUE_MASK,RED_MASK,BLUE_MASK,NONE_MASK,RED_MASK|BLUE_MASK,
      RED_MASK,NONE_MASK,BLUE_MASK,NONE_MASK,BLUE_MASK,BLUE_MASK,RED_MASK,RED_MASK,
      RED_MASK,NONE_MASK,RED_MASK,BLUE_MASK,NONE_MASK,RED_MASK|BLUE_MASK,RED_MASK,RED_MASK,
      RED_MASK|BLUE_MASK,RED_MASK,BLUE_MASK,RED_MASK,RED_MASK|BLUE_MASK,RED_MASK|BLUE_MASK,NONE_MASK,NONE_MASK,
      NONE_MASK,RED_MASK|BLUE_MASK,BLUE_MASK,RED_MASK,NONE_MASK,BLUE_MASK,BLUE_MASK,RED_MASK,
      RED_MASK,RED_MASK|BLUE_MASK,NONE_MASK,RED_MASK,BLUE_MASK,BLUE_MASK,NONE_MASK,NONE_MASK,
      RED_MASK,BLUE_MASK,BLUE_MASK,RED_MASK|BLUE_MASK,RED_MASK|BLUE_MASK,NONE_MASK,RED_MASK,BLUE_MASK,
      RED_MASK,RED_MASK|BLUE_MASK,BLUE_MASK,BLUE_MASK,NONE_MASK,RED_MASK|BLUE_MASK,RED_MASK|BLUE_MASK,NONE_MASK,
      BLUE_MASK,NONE_MASK,RED_MASK,NONE_MASK,RED_MASK|BLUE_MASK,RED_MASK,BLUE_MASK,NONE_MASK,
      RED_MASK|BLUE_MASK,BLUE_MASK,RED_MASK|BLUE_MASK,RED_MASK,RED_MASK|BLUE_MASK,BLUE_MASK,RED_MASK,NONE_MASK
     }
  }
  
  ,
  {
    8 * 4, 
    // dual
    {  RED_MASK, RED_MASK, RED_MASK, RED_MASK, RED_MASK, RED_MASK, RED_MASK, RED_MASK,
       RED_MASK, RED_MASK, RED_MASK, RED_MASK, RED_MASK, RED_MASK, RED_MASK, RED_MASK,
       
       BLUE_MASK, BLUE_MASK, BLUE_MASK, BLUE_MASK, BLUE_MASK, BLUE_MASK, BLUE_MASK, BLUE_MASK,
       BLUE_MASK, BLUE_MASK, BLUE_MASK, BLUE_MASK, BLUE_MASK, BLUE_MASK, BLUE_MASK, BLUE_MASK   
    }
  }
  ,
  {
    64, 
    // 8 pulses of 50 ms for each light
    {  RED_MASK, RED_MASK, NONE_MASK, NONE_MASK, RED_MASK, RED_MASK, NONE_MASK, NONE_MASK, RED_MASK, RED_MASK, NONE_MASK, NONE_MASK, RED_MASK, RED_MASK, NONE_MASK, NONE_MASK,
       RED_MASK, RED_MASK, NONE_MASK, NONE_MASK, RED_MASK, RED_MASK, NONE_MASK, NONE_MASK, RED_MASK, RED_MASK, NONE_MASK, NONE_MASK, RED_MASK, RED_MASK, NONE_MASK, NONE_MASK,
       BLUE_MASK, BLUE_MASK, NONE_MASK, NONE_MASK, BLUE_MASK, BLUE_MASK, NONE_MASK, NONE_MASK, BLUE_MASK, BLUE_MASK, NONE_MASK, NONE_MASK, BLUE_MASK, BLUE_MASK, NONE_MASK, NONE_MASK,
       BLUE_MASK, BLUE_MASK, NONE_MASK, NONE_MASK, BLUE_MASK, BLUE_MASK, NONE_MASK, NONE_MASK, BLUE_MASK, BLUE_MASK, NONE_MASK, NONE_MASK, BLUE_MASK, BLUE_MASK, NONE_MASK, NONE_MASK
    }
  }
  ,
  {
    32, 
    // 4 pulses of 50 ms for each light
    {  RED_MASK, RED_MASK, NONE_MASK, NONE_MASK, RED_MASK, RED_MASK, NONE_MASK, NONE_MASK, RED_MASK, RED_MASK, NONE_MASK, NONE_MASK, RED_MASK, RED_MASK, NONE_MASK, NONE_MASK,
       BLUE_MASK, BLUE_MASK, NONE_MASK, NONE_MASK, BLUE_MASK, BLUE_MASK, NONE_MASK, NONE_MASK, BLUE_MASK, BLUE_MASK, NONE_MASK, NONE_MASK, BLUE_MASK, BLUE_MASK, NONE_MASK, NONE_MASK
    }
  }
  ,
  {
    8, 
    // 1 pulses of 50 ms for each light
    {  RED_MASK, RED_MASK, NONE_MASK, NONE_MASK,
       BLUE_MASK, BLUE_MASK, NONE_MASK, NONE_MASK}
  }
};

// sound
#define TONE_PIN 1
#define TONE_FREQUENCY 1000
#define TONE_DURATION_MS 100

// defining pins the bumper flashes
#define BUMPER_RED 9
#define BUMPER_BLUE 10
#define BUMPER_MODE_ADDR 0x00
#define BUMPER_BUTTON 5

// defining pins for the flashing lights behind turn light
#define WHITE_FLASH_LEFT 11
#define WHITE_FLASH_RIGHT 12
#define WHITE_FLASH_MODE_ADDR 0x01
#define WHITE_FLASH_BUTTON 4

// button for using flash light behind the windshield
#define WINDSHIELD_OUTPUT 13
#define WINDSHIELD_BUTTON 3

// global button for enable all or disable all
#define GLOBAL_BUTTON 2

#define SOUND_PIN  7
#define SOUND_SHORT_PULSE 2
#define SOUND_LONG_PULSE 4

// dual flashing lights
struct dual_flashing_light
{
  byte red_pin;
  byte blue_pin;
  byte mode_addr;
  byte mode;
  byte enable;
  byte counter;
};

// init  flashing light
// disable by default all
// set pin as outputs
struct dual_flashing_light dual_flashing_light_init(byte red_pin, byte blue_pin, byte mode_addr)
{
  struct dual_flashing_light self;

  self.red_pin = red_pin;
  self.blue_pin = blue_pin;
  self.mode_addr = mode_addr;
  self.enable = 0;
  self.counter = 0;
  
  self.mode = EEPROM.read(self.mode_addr);
  
  // prevent trouble if the eeprom contains a random pattern
  if(self.mode >= DUAL_FLASHING_LIGHT_MAX_MODE){
    self.mode = 0;
    EEPROM.write(self.mode_addr, self.mode);
  }

  pinMode(red_pin, OUTPUT);
  pinMode(blue_pin, OUTPUT);

  return self;
}

// enable the dual lights, using the memorized mode
// so if the current was shutdown, the mode will be saved
void dual_flashing_light_enable(struct dual_flashing_light * self)
{
  self->enable = 1; 
  self->mode = EEPROM.read(self->mode_addr);
  self->counter = 0;
}

// just disable the dual lights
void dual_flashing_light_disable(struct dual_flashing_light * self)
{
  self->enable = 0; 
}

// use to toggle dual lights
void dual_flashing_light_toggle(struct dual_flashing_light * self)
{
  if(self->enable == 0){
    dual_flashing_light_enable(self);
  }
  else{
    dual_flashing_light_disable(self);
  }
}

// increment the mode up to the max mode value, and reset the counter
// each time, the new mode is save in EEPROM
void dual_flashing_light_change_mode(struct dual_flashing_light * self)
{
  if(self->enable){
    self->mode++;
    self->counter = 0;

    if(self->mode >= DUAL_FLASHING_LIGHT_MAX_MODE){
      self->mode = 0;
    }

    EEPROM.write(self->mode_addr, self->mode);
  }
}

byte dual_flashing_light_is_enable(struct dual_flashing_light * self){
  return self->enable;
}

// this function is used to set light regarding a mask
void dual_flashing_light_set_leds(struct dual_flashing_light * self, byte mode_mask)
{
  // if the red mask is set, we enable the red light
  if(mode_mask & RED_MASK){
    digitalWrite(self->red_pin, HIGH);
  }
  else{
    digitalWrite(self->red_pin, LOW);
  }

  // if the blue mask is set, we enable the blue light
  if(mode_mask & BLUE_MASK){
    digitalWrite(self->blue_pin, HIGH);
  }
  else{
    digitalWrite(self->blue_pin, LOW);
  }
}

// this function should be call in each loop cycle
void dual_flashing_light_update(struct dual_flashing_light * self)
{
  if(self->enable){
    // getting current led state
    byte pattern_size = DUAL_FLASH_PATTERNS[self->mode].pattern_size;
    byte mask = DUAL_FLASH_PATTERNS[self->mode].pattern_value[self->counter];

    // set led values
    dual_flashing_light_set_leds(self, mask);

    // update internal counter
    self->counter++;
    if(self->counter >= pattern_size){
      self->counter = 0;
    }

    // disable lights
  }
  else{
    dual_flashing_light_set_leds(self, NONE_MASK);
  }
}

// this module generate a pulse for an external buzzer
struct sound_pulse{
  byte pin;
  byte counter;  
};

// set a module that manage a pulse on output
struct sound_pulse sound_pulse_init(byte pin){
  struct sound_pulse self;

  self.pin = pin;
  self.counter = 0;

  pinMode(pin, OUTPUT);
  digitalWrite(pin, true);

  return self;
}

void sound_pulse_set(sound_pulse * self, byte value){
  self->counter = value;
}

void sound_pulse_update(sound_pulse * self){
  if(self->counter > 0){
    self->counter--;
    digitalWrite(self->pin, false);
  }else{
    digitalWrite(SOUND_PIN, true);
  }
}


// this struct manages a push button 
struct button{
  byte pin;
  byte counter;
};

// set the button pin a pullup input
struct button button_init(byte pin){
  struct button self;

  self.pin = pin;
  self.counter = 0;

  pinMode(pin, INPUT_PULLUP);

  return self;
}

// this function should be call in each loop cycle
// short_fct is a callback called for a push between 100ms and 1 sec
// long_fct is a callback called for a push greater than 1 sec
void button_update(struct button* self, byte (*short_fct)(void), byte (*long_fct)(void), sound_pulse * sound){
  if(digitalRead(self->pin) == false){
    // if the counter is not up to the maximum
    if(self->counter <= BUTTON_LONG_PUSH){
      self->counter++;
    }
    
  }
  else{

    if(self->counter < BUTTON_LONG_PUSH){
      if(self->counter > 0){
        // short push case
        if(short_fct){
          if(short_fct() != 0){
            sound_pulse_set(sound, SOUND_SHORT_PULSE);
          }
        }
      }
    }
    else{
      // long push case
      if(long_fct){
        if(long_fct() != 0){
          sound_pulse_set(sound, SOUND_LONG_PULSE);
        }
      }
    } 
    
    self->counter = 0;
  }
}


// var declarations
// the bumper
struct dual_flashing_light bumper_flash;
struct button bumper_button;

// the white flashing light
struct dual_flashing_light white_flash;
struct button white_flash_button;

// command for all functions
struct button global_button;

// about the sound
struct sound_pulse sound;

// callbacks for bumper
// a short push will toggle (on/off) the leds
// a long push will change the mode

byte bumper_short_push(void){
  dual_flashing_light_toggle(&bumper_flash);
  return 1;
}

byte bumper_long_push(void){
  dual_flashing_light_change_mode(&bumper_flash);
  return dual_flashing_light_is_enable(&bumper_flash);
}

// callbacks for white light
// a short push will toggle (on/off) the leds
// a long push will change the mode

byte white_flash_short_push(void){
  dual_flashing_light_toggle(&white_flash);
  return 1;
}

byte white_flash_long_push(void){
  dual_flashing_light_change_mode(&white_flash);
  return dual_flashing_light_is_enable(&white_flash);
}

// callbacks for all functions
// a short push will stop all functions
// a long push will enable all functon on the current mode available

byte global_short_push(void){
  byte start_state = bumper_flash.enable | bumper_flash.enable;
  dual_flashing_light_disable(&bumper_flash);
  dual_flashing_light_disable(&white_flash);
  byte end_state = bumper_flash.enable | bumper_flash.enable;
  return start_state != end_state;
}

byte global_long_push(void){
  byte start_state = bumper_flash.enable | bumper_flash.enable;
  dual_flashing_light_enable(&bumper_flash);
  dual_flashing_light_enable(&white_flash);
  byte end_state = bumper_flash.enable | bumper_flash.enable;
  return start_state != end_state;
}

void setup() 
{                

  // bumper init
  bumper_flash = dual_flashing_light_init(BUMPER_RED, BUMPER_BLUE, BUMPER_MODE_ADDR);
  bumper_button = button_init(BUMPER_BUTTON);
  
  // white flash init
  white_flash = dual_flashing_light_init(WHITE_FLASH_LEFT, WHITE_FLASH_RIGHT, WHITE_FLASH_MODE_ADDR);
  white_flash_button = button_init(WHITE_FLASH_BUTTON);
  
  // global command
  global_button = button_init(GLOBAL_BUTTON);
  
  sound = sound_pulse_init(SOUND_PIN);
  
  // build in test
  sound_pulse_set(&sound, SOUND_SHORT_PULSE);

}

//boolean cycle_state = 0;

void loop() 
{
  
  // get start time to make a diff time
  unsigned long start_time = millis();
  
  // processing the loop
  // buttons
  button_update(&bumper_button, bumper_short_push, bumper_long_push, &sound);
  button_update(&white_flash_button, white_flash_short_push, white_flash_long_push, &sound);
  button_update(&global_button, global_short_push, global_long_push, &sound);
  // leds
  dual_flashing_light_update(&bumper_flash);
  dual_flashing_light_update(&white_flash);
  
  sound_pulse_update(&sound);

  // compute the diff time and wait until 25 ms elapsed
  unsigned long loop_diff = millis() - start_time;
  
  delay(LOOP_DELAY_MS - loop_diff);

  //digitalWrite(CYCLE_LED, cycle_state);
  // cycle_state = 1- cycle_state;
}
