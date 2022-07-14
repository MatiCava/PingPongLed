#include <Wire.h>                 
#include <LiquidCrystal_I2C.h>  

LiquidCrystal_I2C lcd(0x27,16,2);

bool willTheBallGoTowardsPlayerTwo = true;
bool isInputAllowed = true;

const int btn = 0;
const int playerOne = 12;
const int goalPlayerOne = 13;
const int buttonPlayerOne = 3;

const int playerTwo = 5;
const int goalPlayerTwo = 4;
const int buttonPlayerTwo = 2;
const int PIEZO         = 1; 

int scoreOfPlayerOne = 0;
int scoreOfPlayerTwo = 0;

const unsigned long initialMillisecondsPerLED = 500;
const unsigned long initialDeltaMillisecondsPerLED = 50;
unsigned long millisecondsPerLED = initialMillisecondsPerLED;
unsigned long deltaMillisecondsPerLED = initialDeltaMillisecondsPerLED;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

int currentPosition = playerOne;        
int previousPosition = playerOne + 1;
int deltaPosition = 0;

int buttonStatePlayerOne = 0;
int lastButtonStatePlayerOne = 0;
int buttonStatePlayerTwo = 0;
int lastButtonStatePlayerTwo = 0;

void setup() {
   lcd.init();                     
  lcd.backlight();
  pinMode(1, 1);
  pinMode(4, 1);
  pinMode(5, 1);
  pinMode(6, 1);
  pinMode(7, 1);
  pinMode(8, 1);
  pinMode(9, 1);
  pinMode(10, 1);
  pinMode(11, 1);
  pinMode(12, 1);
  pinMode(13, 1);

  pinMode(2, 0);   
  pinMode(3, 0);   
}

void loop()
{
  ListenForInput();
  currentMillis = millis();
  if (currentMillis - previousMillis >= millisecondsPerLED)  
  {
    CheckGoalConditions();
    DetermineNextPosition();
    MoveBallToNextPosition();

    previousMillis = currentMillis;
  }
 }

void ListenForInput()    
{
  buttonStatePlayerOne = digitalRead(buttonPlayerOne);
  buttonStatePlayerTwo = digitalRead(buttonPlayerTwo);

  if (buttonStatePlayerOne != lastButtonStatePlayerOne && isInputAllowed)
  {
    if (buttonStatePlayerOne == 1)
    {
      if (currentPosition == playerOne)
      {
        ToggleBallDirection();
        IncreaseSpeed();
      }
      else
      {
        ScoreForPlayer(2);
      }
    }
    lastButtonStatePlayerOne = buttonStatePlayerOne;
  }
  if (buttonStatePlayerTwo != lastButtonStatePlayerTwo && isInputAllowed)
  {
    if (buttonStatePlayerTwo == 1)
    {
      if (currentPosition == playerTwo)
      {
        ToggleBallDirection();
        IncreaseSpeed();
      }
      else
      {
        ScoreForPlayer(1);
      }
    }
    lastButtonStatePlayerTwo = buttonStatePlayerTwo;
  }
}

void ToggleBallDirection()
{
  willTheBallGoTowardsPlayerTwo = !willTheBallGoTowardsPlayerTwo;
  isInputAllowed = false;   
}

void IncreaseSpeed()
{
  millisecondsPerLED -= deltaMillisecondsPerLED;
  if (deltaMillisecondsPerLED > 5)  
  {
    deltaMillisecondsPerLED -= 5; 
  }
}

void MoveBallToNextPosition()   
{
  previousPosition = currentPosition;
  digitalWrite(previousPosition, 0);
  currentPosition = currentPosition + deltaPosition;
  digitalWrite(currentPosition, 1);
  isInputAllowed = true;
}

void DetermineNextPosition()
{
  if (willTheBallGoTowardsPlayerTwo)
  {
    deltaPosition = -1;
  }
  else
  {
    deltaPosition = 1;
  }
}

void CheckGoalConditions()
{
  if (currentPosition == goalPlayerTwo)
  {
    ScoreForPlayer(1);

  }
  else if (currentPosition == goalPlayerOne)
  {
    ScoreForPlayer(2);
  }
}

void ScoreForPlayer(int playerWhoScored)
{
  isInputAllowed = false;
  FlashAllLEDs(1, 0);
  if (playerWhoScored == 1)
  {
    scoreOfPlayerOne++;
    ShowScores(1);
  }
  else if (playerWhoScored == 2)
  {
    scoreOfPlayerTwo++;
    ShowScores(2);
  }
  CheckEndGame();
}

void resetLCD(){
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
}

void CheckEndGame()
{
  if (scoreOfPlayerOne == 3)
  {
    resetLCD();
    EndGameCeremonyFor(2);
    lcd.setCursor(0, 0); 
    lcd.print("player one wins "); 
    lcd.setCursor(0, 1);
    lcd.print("congratulations ");
    delay(2500);
    soundCorrectGuess();
    lcd.setCursor(0, 0); 
    lcd.print("player 2,       "); 
    lcd.setCursor(0, 1);
    lcd.print("play again      ");
    delay(2500);
     soundBuzzer ();
     lcd.setCursor(0, 0); 
    lcd.print("                "); 
    lcd.setCursor(0, 1);
    lcd.print("                ");
    delay(2500);
  }
  if (scoreOfPlayerTwo == 3)
  {
    resetLCD();
    EndGameCeremonyFor(1);
    lcd.setCursor(0, 0); 
    lcd.print("player two wins "); 
    lcd.setCursor(0, 1);
    lcd.print("congratulations ");
    delay(2500);
    soundCorrectGuess();
    lcd.setCursor(0, 0); 
    lcd.print("player 1,       "); 
    lcd.setCursor(0, 1);
    lcd.print("play again      ");
    delay(2500);
    soundBuzzer ();
     lcd.setCursor(0, 0); 
    lcd.print("                "); 
    lcd.setCursor(0, 1);
    lcd.print("                ");
    delay(2500);
  }
}

void showLCDGoal(int winner){
  lcd.setCursor(0, 0); 
  String scores = "     " + String(scoreOfPlayerTwo) + " - " + String(scoreOfPlayerOne);
  lcd.print(scores);
  lcd.setCursor(0, 1);
  String striker = "player " + String(winner) + " scored!";
  lcd.print(striker);
}

void ShowScores(int playerCurrentlyScored)
{
  if (playerCurrentlyScored == 1)
  {
    digitalWrite(playerOne, 1);
    currentPosition = playerOne;
    willTheBallGoTowardsPlayerTwo = true;
  }
  else if (playerCurrentlyScored == 2)
  {
    digitalWrite(playerTwo, 1);
    currentPosition = playerTwo;
    willTheBallGoTowardsPlayerTwo = false;
  }

  for (int i = 0; i < scoreOfPlayerTwo; i++) 
  {
    digitalWrite((6 + i), 1);
  }
  for (int i = 0; i < scoreOfPlayerOne; i++)
  {
    digitalWrite((11 - i), 1);
  }

  showLCDGoal(playerCurrentlyScored);
  delay(3000);                  
  ResetValuesForNextRound();
}

void ResetValuesForNextRound() 
{
  FlashAllLEDs(1, 0);
  millisecondsPerLED = initialMillisecondsPerLED;          
  deltaMillisecondsPerLED = initialDeltaMillisecondsPerLED;
  lcd.setCursor(0, 1);
  lcd.print("                ");
}
void EndGameCeremonyFor(int winner)
{
  FlashAllLEDs(50, winner);
  TurnOffAllLEDsForPlayer(0);
  scoreOfPlayerOne = 0;
  scoreOfPlayerTwo = 0;
}

void TurnOnAllLEDsForPlayer(int player)
{
  if (player != 1)          
  {
    digitalWrite(9, 1);
    digitalWrite(10, 1);
    digitalWrite(11, 1);
    digitalWrite(12, 1);
    digitalWrite(13, 1);
  }
  if (player != 2)          
  {
    digitalWrite(4, 1);
    digitalWrite(5, 1);
    digitalWrite(6, 1);
    digitalWrite(7, 1);
    digitalWrite(8, 1);
  }
}

void TurnOffAllLEDsForPlayer(int player)
{
  if (player != 1)          
  {
    digitalWrite(4, 0);
    digitalWrite(5, 0);
    digitalWrite(6, 0);
    digitalWrite(7, 0);
    digitalWrite(8, 0);
  }
  if (player != 2)         
  {
    digitalWrite(9, 0);
    digitalWrite(10, 0);
    digitalWrite(11, 0);
    digitalWrite(12, 0);
    digitalWrite(13, 0);
  }
}

void FlashAllLEDs(int blinkCount, int player) 
{
  for (int i = 0; i < blinkCount; i++)
  {
    TurnOnAllLEDsForPlayer(player);
    delay(35);
    TurnOffAllLEDsForPlayer(player);
    delay(35);
  }
}
void soundBuzzer() {
  tone(PIEZO, 100, 2000);
  delay(2300);  
}

void soundCorrectGuess() {
  tone(PIEZO, 700, 100);
  delay(100);
  tone(PIEZO, 800, 100);
  delay(100);
  tone(PIEZO, 900, 100);
  delay(100);
  tone(PIEZO, 1000, 100);
  delay(100);
  tone(PIEZO, 1100, 100);
  delay(100);
  tone(PIEZO, 1200, 100);
  delay(100);
}
