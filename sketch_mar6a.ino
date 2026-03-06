const int BUTTON_PIN = 2;   // Hardware Interrupt Pin
const int HAPTIC_PIN = 13;  // Built-in LED for vibration

// RGB LED Pins
const int LED_RED = 11;
const int LED_GREEN = 10;
const int LED_BLUE = 9;

// 'volatile' is required for variables modified inside an Interrupt
volatile int spikeCount = 0;
volatile unsigned long firstSpikeTime = 0;
volatile unsigned long lastInterruptTime = 0;
volatile bool newSpike = false;

// The "Guardian AI" Thresholds
const int REQUIRED_SPIKES = 5;
const int TIME_WINDOW = 2000; // 2 seconds

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(HAPTIC_PIN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  Serial.begin(9600);
  Serial.println("Guardian Ring: Armed.");
  Serial.println("CPU entering low-power monitoring state...");

  // Initial State: GREEN (Safe)
  setRGB(0, 255, 0); 

  // Attach the Interrupt: Triggers instantly when the button drops to LOW
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), motionDetectedISR, FALLING);
}

// ---------------------------------------------------------
// INTERRUPT SERVICE ROUTINE (The "Wake Up" Call)
// This runs instantly, pausing everything else. Must be fast!
// ---------------------------------------------------------
void motionDetectedISR() {
  unsigned long interruptTime = millis();
  
  // Anti-bounce filter: Ignore phantom clicks faster than 150ms
  if (interruptTime - lastInterruptTime > 150) {
    if (spikeCount == 0) {
      firstSpikeTime = interruptTime;
    }
    spikeCount++;
    newSpike = true;
    lastInterruptTime = interruptTime;
  }
}

// ---------------------------------------------------------
// MAIN LOOP (The "Brain")
// ---------------------------------------------------------
void loop() {
  // 1. If the Interrupt woke us up and registered a spike:
  if (newSpike) {
    Serial.print("Motion Spike! Danger Score: ");
    Serial.println(spikeCount);
    
    // Change LED to BLUE to show it is actively analyzing a struggle
    setRGB(0, 0, 255); 
    newSpike = false;
  }

  // 2. Window Checking Logic
  if (spikeCount > 0) {
    // Has the 2-second window expired?
    if (millis() - firstSpikeTime > TIME_WINDOW) {
      
      if (spikeCount >= REQUIRED_SPIKES) {
        triggerSOS();
      } else {
        Serial.println("Pattern degraded. False alarm averted.");
        Serial.println("CPU returning to low-power state...");
        spikeCount = 0;
        setRGB(0, 255, 0); // Back to GREEN (Safe)
      }
    }
  }
}

// ---------------------------------------------------------
// SOS TRIGGER & LED HELPER
// ---------------------------------------------------------
void triggerSOS() {
  Serial.println("\n🚨 SOS TRIGGERED: Transmitting Location Data! 🚨");
  
  // Turn LED RED and trigger HAPTIC (Pin 13)
  setRGB(255, 0, 0);
  digitalWrite(HAPTIC_PIN, HIGH);
  
  delay(3000); // Hold for 3 seconds
  
  // Reset System
  digitalWrite(HAPTIC_PIN, LOW);
  spikeCount = 0;
  Serial.println("\nGuardian Ring: Armed.");
  setRGB(0, 255, 0); // Back to GREEN
}

// Helper function to easily change LED colors
void setRGB(int r, int g, int b) {
  analogWrite(LED_RED, r);
  analogWrite(LED_GREEN, g);
  analogWrite(LED_BLUE, b);
}