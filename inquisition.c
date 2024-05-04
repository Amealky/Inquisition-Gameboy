
#include <gb/gb.h>
#include <gb/drawing.h>

#include <gbdk/console.h>

#include <rand.h>
#include <stdio.h>

#include "graphics.h"

#define MAX_SPRITE 16
#define MAX_MOB_SPRITE 4
#define NB_BANDIT 3
#define BANDIT_RUN 1
#define BANDIT_FOLLOW 2
#define MAX_TIMER_SCROLLBKG 10
#define HAMMER_DISTANCE 8

#define MAX_TIMER 10
#define MAX_TIMER_BANDIT 10

typedef struct Bandit{
  UBYTE pv;
  UBYTE PosX, PosY;
  UBYTE speed;
  UBYTE damage;
  UBYTE t;
  UBYTE comportement;
  UBYTE timerAnimationWalkBandit;
  UBYTE timerAnimationAttackBandit;
  UBYTE timerAnimationDyingBandit;
  UBYTE isWalkingBandit;
  UBYTE isAttackingBandit;
  UBYTE isDyingBandit;
  UBYTE isNotHere;

} Bandit;

Bandit bandits[NB_BANDIT];
UBYTE pickComportement;

UBYTE nbBanditToDraw = 3;

UBYTE posXJudge = 80;
UBYTE posYJudge = 100;

UBYTE posXShock = 80;
UBYTE posYShock = 100;

UBYTE shockSend = 0;

UBYTE JudgeSpeed = 4;
UBYTE JudgePV = 3;

UBYTE TimerAnimationWalk = MAX_TIMER;
UBYTE TimerAnimationJump = MAX_TIMER;
UBYTE TimerAnimationAttack = MAX_TIMER;
UBYTE TimerAnimationHurt = MAX_TIMER/2;
UBYTE TimerScrollBKG = MAX_TIMER_SCROLLBKG;

UBYTE isWalking = 0;
UBYTE isAttacking = 0;
UBYTE isHurt = 0;
UBYTE i = 0;
UBYTE j = 0;
UBYTE draw_i = 0;

UBYTE borderRight = SCREENWIDTH-4;
UBYTE borderLeft = 4;
UBYTE borderTop = 45;
UBYTE borderBottom = SCREENHEIGHT-38;

UBYTE hammerPos = 80 + HAMMER_DISTANCE;

UBYTE score_bandit_digitUn = 0;
UBYTE score_bandit_digitDeux = 0;
UBYTE score_bandit_digitTrois = 0;
UBYTE score_bandit = 0;


UBYTE score_villager = 10;
UBYTE score_villager_digitUn = 0;
UBYTE score_villager_digitDeux = 1;

UBYTE gameIsLaunch = 0;
UBYTE isGameOver = 0;
UBYTE bestScore = 0;



void sound_atk(){
  NR52_REG = 0x80;
  NR51_REG = 0x11;
  NR50_REG = 0x77;

  NR10_REG = 0x1E;
  NR11_REG = 0x10;
  NR12_REG = 0xF3;
  NR13_REG = 0x00;
  NR14_REG = 0x87;
}


/********************************************************************
 * ================================================================ *
*********************************************************************/


/***
 * INIT FUNCTIONS
*/
void initialSetup();
void initGameDatas();
void initBandits();

/***
 * SCREEN FUNCTIONS
*/
void showNameSystemLogo();
void showBenjaminMetautLogo();
void showBootScreens();
void showStartScreen();
void showGameScreen();
void showScoreScreen();

/***
 * DRAW FUNCTIONS
*/
void setSpritesTiles(UBYTE start, UBYTE nb);
void drawJudgeAttack1();
void drawJudgeAttack2();
void drawJudgeIDLE();
void drawJudgeWalk1();
void drawJudgeWalk2();
void drawJudgeHurt();
void drawjudgeShockWave();
void drawShock();
void drawBanditIdle(UBYTE numero, UBYTE posX, UBYTE posY);
void drawBanditWalk1(UBYTE numero, UBYTE posX, UBYTE posY);
void drawBanditWalk2(UBYTE numero, UBYTE posX, UBYTE posY);
void drawBanditAttack1(UBYTE numero, UBYTE posX, UBYTE posY);
void drawBanditAttack2(UBYTE numero, UBYTE posX, UBYTE posY);
void drawBanditDead(UBYTE numero, UBYTE posX, UBYTE posY);
void drawPv();
void drawScoreBandit();
void drawScoreVillager();


/***
 * COLLISION FUNCTIONS
*/
void checkCollideBorder(UBYTE newPosX, UBYTE newPosY);
void checkColliderHammerAttack();
void checkCollideShockAttack();


/***
 * BEHAVIOR FUNCTIONS
*/
void JudgeDoHurt();
void JudgeWalk();
void JudgeGroundAttack();
void JudgeSendSentence();
void BanditWalk(UBYTE numero, UBYTE posX, UBYTE posY);
void BanditAttack(UBYTE numero, UBYTE posX, UBYTE posY);
void BanditDie(UBYTE numero, UBYTE posX, UBYTE posY);

/***
 * MOVE FUNCTIONS
*/
void moveJudge();
void moveShockWave();
void moveBandits();
void scrollCity();

/***
 * CHORE FUNCTIONS
*/
void clearBkgAndWin();
void generateBanditsST();
void generateBandits();
void calculateScoreVillager();
void checkBanditScore();


/********************************************************************
 * ================================================================ *
*********************************************************************/



/***
 * INIT FUNCTIONS IMPL
*/
void initialSetup() {
  UWORD seed;
  seed = DIV_REG;
  initarand(seed);

  SPRITES_8x8;

  NR52_REG = 0xF8U;
  NR51_REG = 0x00U;
  NR50_REG = 0x77U;

  clearBkgAndWin();
}

void initGameDatas() {

  isGameOver = 0;
  gameIsLaunch = 1;
  shockSend = 0;
  JudgePV = 3;
  score_villager = 10;
  score_villager_digitUn = 0;
  score_villager_digitDeux = 1;

  score_bandit_digitUn = 0;
  score_bandit_digitDeux = 0;
  score_bandit_digitTrois = 0;
  score_bandit = 0;

  posXJudge = 80;
  posYJudge = 100;

  posXShock = posXJudge;
  posYShock = posYJudge;

}

void initBandits() {

  for(i = 0; i < NB_BANDIT; i++){
    bandits[i].pv = 0;
    bandits[i].speed = 1;
    bandits[i].damage = 1;
    bandits[i].PosX = -5;
    bandits[i].PosY = -5;
    bandits[i].t = i+1;
    bandits[i].comportement = 0;
    bandits[i].timerAnimationWalkBandit = MAX_TIMER_BANDIT;
    bandits[i].timerAnimationAttackBandit = MAX_TIMER_BANDIT;
    bandits[i].timerAnimationDyingBandit = MAX_TIMER_BANDIT;
    bandits[i].isAttackingBandit = 0;
    bandits[i].isWalkingBandit = 0;
    bandits[i].isDyingBandit = 0;
  }

}


/***
 * SCREEN FUNCTIONS IMPL
*/
void showNameSystemLogo() {
  set_bkg_data(0, 15, logoTiles);
  set_win_tiles(0, 0, 7, 3, logoWin);
  move_win(SCREENWIDTH/2 - (3*8), SCREENHEIGHT/2 - (2*8)); 
}

void showBenjaminMetautLogo() {
  set_bkg_data(0, 14, BenMetTiles);
  set_win_tiles(0, 0, 9, 4, BenMetLogo);
  move_win(SCREENWIDTH/2 - (3*8), SCREENHEIGHT/2 - (2*8)); //HUD
}

void showBootScreens() {

  showNameSystemLogo();

  SHOW_BKG;
  SHOW_WIN;
  DISPLAY_ON;

  delay(2000);
  wait_vbl_done();

  HIDE_BKG;
  HIDE_WIN;
  DISPLAY_OFF;

  clearBkgAndWin();

  showBenjaminMetautLogo();

  SHOW_BKG;
  SHOW_WIN;
  DISPLAY_ON;

  delay(2000);
  wait_vbl_done();

}

void showStartScreen() {

  HIDE_WIN;
  SHOW_BKG;
  SHOW_SPRITES;
  DISPLAY_ON;

  set_bkg_data(0, 43 + 41, decors_data);
  set_bkg_tiles(0, 0, 20, 20, TitleScreen);

  do {
   generateBanditsST();
   moveBandits();
  } while(joypad() != J_START);

}

void showGameScreen() {

  SHOW_SPRITES;
  SHOW_WIN;
  SHOW_BKG;
  DISPLAY_ON;

  initGameDatas();
  initBandits();

  set_bkg_data(0, 104, decors_data);
  set_bkg_tiles(0, 0, 32, 16, CityMap);
  set_win_tiles(0, 0, 20, 2, HUD_MAP);


  move_win(7, borderBottom + 22); //HUD

  while(isGameOver == 0){
      drawScoreBandit();
      drawPv();
      drawShock();
      calculateScoreVillager();
      drawScoreVillager();
      scrollCity();
      generateBandits();
      moveJudge();
      moveBandits();
      if(shockSend == 1){
        moveShockWave();
      }
      if(isAttacking == 1){
        JudgeGroundAttack();
      }

      if(isWalking == 0 && isAttacking == 0 && isHurt == 0){
        drawJudgeIDLE();
      }

      
  }

}

void showScoreScreen() {
  HIDE_SPRITES;
  HIDE_WIN;


  //Reset judge position outside of the screen
  posXJudge = borderLeft - 50;
  posYJudge = borderTop - 50;
  posXShock = posXJudge;
  posYJudge = posYJudge;
  drawJudgeIDLE();


  gameIsLaunch = 0;

  if(score_bandit > bestScore){
    bestScore = score_bandit;
  }

  set_bkg_data(0, 104, decors_data);

  set_bkg_tiles(0, 0, 20, 20, ScoreScreen);
  move_bkg(0, 0); //HUD

  do{
  }while(joypad() != J_A);

}


/***
 * DRAW FUNCTIONS IMPL
*/
void setSpritesTiles(UBYTE start, UBYTE nb) {
  for(draw_i = 0; draw_i < nb; draw_i++) {
    set_sprite_tile(start+draw_i, start+draw_i);
  }
}

void drawJudgeAttack1() {

  set_sprite_data(0, MAX_SPRITE, JUDGEATTACK1);

  setSpritesTiles(0, 16);

  move_sprite(0, posXJudge-8, posYJudge-8);
  move_sprite(1, posXJudge-8, posYJudge);
  move_sprite(3, posXJudge, posYJudge);
  move_sprite(4, posXJudge-8, posYJudge+8);
  move_sprite(5, posXJudge-8, posYJudge+16);
  move_sprite(6, posXJudge, posYJudge+8);
  move_sprite(7, posXJudge, posYJudge+16);
  move_sprite(8, -5, -5);
  move_sprite(9, -5, -5);
  move_sprite(10, -5, -5);
  move_sprite(11, -5, -5);
  move_sprite(12, posXJudge+8, posYJudge+8);
  move_sprite(13, posXJudge+8, posYJudge+16);
  move_sprite(14, -5, -5);
  move_sprite(15, -5, -5);

  if(shockSend == 0){
    set_win_tiles(3, 0, 1, 1, attackLook);
  }


}

void drawJudgeAttack2() {
  set_sprite_data(0, MAX_SPRITE, JUDGEATTACK2);

  setSpritesTiles(0, 16);
  move_sprite(0, posXJudge-8, posYJudge-8);
  move_sprite(1, posXJudge-8, posYJudge);
  move_sprite(2, -5, -5);
  move_sprite(3, posXJudge, posYJudge);
  move_sprite(4, posXJudge-8, posYJudge+8);
  move_sprite(5, posXJudge-8, posYJudge+16);
  move_sprite(6, posXJudge, posYJudge+8);
  move_sprite(7, posXJudge, posYJudge+16);
  move_sprite(8, -5, -5);
  move_sprite(9, posXJudge+8, posYJudge);
  move_sprite(10, -5, -5);
  move_sprite(11, -5, -5);
  move_sprite(12, posXJudge+8, posYJudge+8);
  move_sprite(13, posXJudge+8, posYJudge+16);
  move_sprite(14, posXJudge+16, posYJudge+8);
  move_sprite(15, posXJudge+16, posYJudge+16);

  if(shockSend == 0){
      set_win_tiles(3, 0, 1, 1, attackLook);
  }

}

void drawJudgeIDLE() {

  set_sprite_data(0, MAX_SPRITE, JUDGEIDLE);

  setSpritesTiles(0, 16);
  move_sprite(0, posXJudge-8, posYJudge-8);

  move_sprite(1, posXJudge-8, posYJudge);
  move_sprite(2, -5, -5);
  move_sprite(3, posXJudge, posYJudge);
  move_sprite(4, posXJudge-8, posYJudge+8);
  move_sprite(5, posXJudge-8, posYJudge+16);
  move_sprite(6, posXJudge, posYJudge+8);
  move_sprite(7, posXJudge, posYJudge+16);
  move_sprite(8, -5, -5);
  move_sprite(9, -5, -5);
  move_sprite(10, -5, -5);
  move_sprite(11, -5, -5);
  move_sprite(12, posXJudge+8, posYJudge+8);
  move_sprite(13, posXJudge+8, posYJudge+16);
  move_sprite(14, -5, -5);
  move_sprite(15, -5, -5);

  set_win_tiles(3, 0, 1, 1, simpleLook);
}

void drawJudgeWalk1() {

  set_sprite_data(0, MAX_SPRITE, JUDGEWALK1);
  setSpritesTiles(0, 16);

  move_sprite(0, posXJudge-8, posYJudge-8);
  move_sprite(1, posXJudge-8, posYJudge);
  move_sprite(2, -5, -5);
  move_sprite(3, posXJudge, posYJudge);
  move_sprite(4, posXJudge-8, posYJudge+8);
  move_sprite(5, posXJudge-8, posYJudge+16);
  move_sprite(6, posXJudge, posYJudge+8);
  move_sprite(7, posXJudge, posYJudge+16);
  move_sprite(8, -5, -5);
  move_sprite(9, -5, -5);
  move_sprite(10, -5, -5);
  move_sprite(11, -5, -5);
  move_sprite(12, posXJudge+8, posYJudge+8);
  move_sprite(13, posXJudge+8, posYJudge+16);
  move_sprite(14, -5, -5);
  move_sprite(15, -5, -5);

  set_win_tiles(3, 0, 1, 1, simpleLook);


}

void drawJudgeWalk2() {

  set_sprite_data(0, MAX_SPRITE, JUDGEWALK2);
  setSpritesTiles(0, 16);

  move_sprite(0, posXJudge-8, posYJudge-8);
  move_sprite(1, posXJudge-8, posYJudge);
  move_sprite(2, -5, -5);
  move_sprite(3, posXJudge, posYJudge);
  move_sprite(4, posXJudge-8, posYJudge+8);
  move_sprite(5, posXJudge-8, posYJudge+16);
  move_sprite(6, posXJudge, posYJudge+8);
  move_sprite(7, posXJudge, posYJudge+16);
  move_sprite(8, -5, -5);
  move_sprite(9, -5, -5);
  move_sprite(10, -5, -5);
  move_sprite(11, -5, -5);
  move_sprite(12, posXJudge+8, posYJudge+8);
  move_sprite(13, posXJudge+8, posYJudge+16);
  move_sprite(14, -5, -5);
  move_sprite(15, -5, -5);
}

void drawJudgeHurt() {

  set_sprite_data(0, MAX_SPRITE, JUDGEHURT);
  setSpritesTiles(0, 16);

  move_sprite(0, posXJudge-8, posYJudge-8);
  move_sprite(1, posXJudge-8, posYJudge);
  move_sprite(2, -5, -5);
  move_sprite(3, posXJudge, posYJudge);
  move_sprite(4, posXJudge-8, posYJudge+8);
  move_sprite(5, posXJudge-8, posYJudge+16);
  move_sprite(6, posXJudge, posYJudge+8);
  move_sprite(7, posXJudge, posYJudge+16);
  move_sprite(8, -5, -5);
  move_sprite(9, posXJudge+8, posYJudge);
  move_sprite(10, -5, -5);
  move_sprite(11, -5, -5);
  move_sprite(12, posXJudge+8, posYJudge+8);
  move_sprite(13, posXJudge+8, posYJudge+16);
  move_sprite(14, -5, -5);
  move_sprite(15, -5, -5);

  set_win_tiles(3, 0, 1, 1, hurtLook);

}

void drawjudgeShockWave() {
  set_sprite_data(32, 1, Shock);

  set_sprite_tile(32, 32);

  posXShock = posXJudge+18;
  posYShock = (posYJudge+14);

  move_sprite(32, posXShock ,posYShock);

  set_win_tiles(3, 0, 1, 1, jumpLook);
}

void drawShock() {
    if(shockSend == 1){
      set_win_tiles(41, 0, 1, 1, emptyIcon);
    }else{
      set_win_tiles(41, 0, 1, 1, shockIcon);
    }
}

void drawBanditIdle(UBYTE numero, UBYTE posX, UBYTE posY){
  set_sprite_data(MAX_SPRITE+MAX_MOB_SPRITE*numero, MAX_MOB_SPRITE, banditIDLE);

  setSpritesTiles(MAX_SPRITE+MAX_MOB_SPRITE*numero, 4);

  move_sprite(MAX_SPRITE+MAX_MOB_SPRITE*numero, posX, posY-8);
  move_sprite((MAX_SPRITE+MAX_MOB_SPRITE*numero)+1, posX, posY);
  move_sprite((MAX_SPRITE+MAX_MOB_SPRITE*numero)+2, posX+8, posY-8);
  move_sprite((MAX_SPRITE+MAX_MOB_SPRITE*numero)+3, posX+8, posY );
}

void drawBanditWalk1(UBYTE numero, UBYTE posX, UBYTE posY) {
  set_sprite_data(MAX_SPRITE+MAX_MOB_SPRITE*numero, MAX_MOB_SPRITE, banditWalk1);

  setSpritesTiles((MAX_SPRITE+MAX_MOB_SPRITE*numero), 4);

  move_sprite(MAX_SPRITE+MAX_MOB_SPRITE*numero, posX, posY-8);
  move_sprite((MAX_SPRITE+MAX_MOB_SPRITE*numero)+1, posX, posY);
  move_sprite((MAX_SPRITE+MAX_MOB_SPRITE*numero)+2, posX+8, posY-8);
  move_sprite((MAX_SPRITE+MAX_MOB_SPRITE*numero)+3, posX+8, posY );

}

void drawBanditWalk2(UBYTE numero, UBYTE posX, UBYTE posY) {
  set_sprite_data(MAX_SPRITE+MAX_MOB_SPRITE*numero, MAX_MOB_SPRITE, banditWalk2);
  
  setSpritesTiles(MAX_SPRITE+MAX_MOB_SPRITE*numero, 4);

  move_sprite(MAX_SPRITE+MAX_MOB_SPRITE*numero, posX, posY-8);
  move_sprite((MAX_SPRITE+MAX_MOB_SPRITE*numero)+1, posX, posY);
  move_sprite((MAX_SPRITE+MAX_MOB_SPRITE*numero)+2, posX+8, posY-8);
  move_sprite((MAX_SPRITE+MAX_MOB_SPRITE*numero)+3, posX+8, posY );

}

void drawBanditAttack1(UBYTE numero, UBYTE posX, UBYTE posY) {
  set_sprite_data(MAX_SPRITE+MAX_MOB_SPRITE*numero, MAX_MOB_SPRITE, banditAttack1);

  setSpritesTiles(MAX_SPRITE+MAX_MOB_SPRITE*numero, 4);

  move_sprite(MAX_SPRITE+MAX_MOB_SPRITE*numero, posX, posY-8);
  move_sprite((MAX_SPRITE+MAX_MOB_SPRITE*numero)+1, posX, posY);
  move_sprite((MAX_SPRITE+MAX_MOB_SPRITE*numero)+2, posX+8, posY-8);
  move_sprite((MAX_SPRITE+MAX_MOB_SPRITE*numero)+3, posX+8, posY );

}

void drawBanditAttack2(UBYTE numero, UBYTE posX, UBYTE posY) {
  set_sprite_data(MAX_SPRITE+MAX_MOB_SPRITE*numero, MAX_MOB_SPRITE, banditAttack2);

  setSpritesTiles(MAX_SPRITE+MAX_MOB_SPRITE*numero, 4);

  move_sprite(MAX_SPRITE+MAX_MOB_SPRITE*numero, posX, posY-8);
  move_sprite((MAX_SPRITE+MAX_MOB_SPRITE*numero)+1, posX, posY);
  move_sprite((MAX_SPRITE+MAX_MOB_SPRITE*numero)+2, posX+8, posY-8);
  move_sprite((MAX_SPRITE+MAX_MOB_SPRITE*numero)+3, posX+8, posY );

}

void drawBanditDead(UBYTE numero, UBYTE posX, UBYTE posY) {
  set_sprite_data(MAX_SPRITE+MAX_MOB_SPRITE*numero, MAX_MOB_SPRITE, banditDead);
  
  setSpritesTiles(MAX_SPRITE+MAX_MOB_SPRITE*numero, 4);

  move_sprite(MAX_SPRITE+MAX_MOB_SPRITE*numero, posX, posY-8);
  move_sprite((MAX_SPRITE+MAX_MOB_SPRITE*numero)+1, posX, posY);
  move_sprite((MAX_SPRITE+MAX_MOB_SPRITE*numero)+2, posX+8, posY-8);
  move_sprite((MAX_SPRITE+MAX_MOB_SPRITE*numero)+3, posX+8, posY );

}

void drawPv() {
  switch(JudgePV){
    case 3:
    set_win_tiles(39, 0, 1, 1, heartFill);
    set_win_tiles(38, 0, 1, 1, heartFill);
    set_win_tiles(37, 0, 1, 1, heartFill);
    break;

    case 2:
    set_win_tiles(39, 0, 1, 1, heartEmpty);
    set_win_tiles(38, 0, 1, 1, heartFill);
    set_win_tiles(37, 0, 1, 1, heartFill);
    break;

    case 1:
    set_win_tiles(39, 0, 1, 1, heartEmpty);
    set_win_tiles(38, 0, 1, 1, heartEmpty);
    set_win_tiles(37, 0, 1, 1, heartFill);
    break;

    case 0:
    set_win_tiles(39, 0, 1, 1, heartEmpty);
    set_win_tiles(38, 0, 1, 1, heartEmpty);
    set_win_tiles(37, 0, 1, 1, heartEmpty);
    isGameOver = 1;
    break;
  }

}

void drawScoreBandit() {

  set_win_tiles(50, 0, 1, 1, zero+score_bandit_digitUn);
  set_win_tiles(49, 0, 1, 1, zero+score_bandit_digitDeux);
  set_win_tiles(48, 0, 1, 1, zero+score_bandit_digitTrois);

}

void drawScoreVillager() {
  set_win_tiles(45, 0, 1, 1, zero+score_villager_digitUn);
  set_win_tiles(44, 0, 1, 1, zero+score_villager_digitDeux);
}


/***
 * COLLISION FUNCTIONS IMPL
*/
void checkCollideBorder(UBYTE newPosX, UBYTE newPosY) {

  if(newPosY < borderTop){
    posYJudge = borderTop;
  }

  if(newPosY > borderBottom){
    posYJudge = borderBottom;
  }


  if(newPosX > borderRight){
    posXJudge = borderRight;
  }

  if(newPosX < borderLeft){
    posXJudge = borderLeft;
  }

}

void checkColliderHammerAttack() {

  for(i = 0; i < nbBanditToDraw; i++){
    if(bandits[i].pv == 1 && bandits[i].isDyingBandit == 0){
      if(bandits[i].PosX < posXJudge+8 && bandits[i].PosX > posXJudge && bandits[i].PosY < posYJudge+24  && bandits[i].PosY > posYJudge+8){
        bandits[i].isDyingBandit = 1;

        score_bandit++;
        score_bandit_digitUn++;
        checkBanditScore();

      }
    }
  }

}

void checkCollideShockAttack() {

    for(i = 0; i < nbBanditToDraw; i++){
       if(bandits[i].pv == 1 && bandits[i].isDyingBandit == 0){
          if(bandits[i].PosX < posXShock+8 && bandits[i].PosX > posXShock && bandits[i].PosY < posYShock+16  && bandits[i].PosY > posYShock-8){
            bandits[i].isDyingBandit = 1;
            score_bandit++;
            score_bandit_digitUn++;
            checkBanditScore();
          }
        }
      }

      if(posXShock > borderRight + 10){
        shockSend = 0;
      }

}


/***
 * BEHAVIOR FUNCTIONS IMPL
*/
void JudgeDoHurt() {
  isHurt = 1;

  if(isAttacking == 0){
    drawJudgeHurt();
  }

  TimerAnimationHurt--;

  if(TimerAnimationHurt <= 0){
    TimerAnimationHurt = MAX_TIMER/2;
    isHurt = 0;
    JudgePV--;
    posXJudge -= 8;
    drawJudgeHurt();
  }

}

void JudgeWalk() {

  isWalking = 1;

  if(isAttacking == 0 && isHurt == 0){
    if(TimerAnimationWalk >= MAX_TIMER/2){
      drawJudgeWalk1();
    } else if(TimerAnimationWalk >= 0 && TimerAnimationWalk < MAX_TIMER/2){
      drawJudgeWalk2();
    }

    TimerAnimationWalk--;

    if (TimerAnimationWalk <= 0){

      TimerAnimationWalk = MAX_TIMER;

    }
  }

}

void JudgeGroundAttack() {
  isAttacking = 1;

  if(TimerAnimationAttack >= MAX_TIMER/2){
    drawJudgeAttack1();
  } else if(TimerAnimationAttack >= 0 && TimerAnimationAttack < MAX_TIMER/2){
    drawJudgeAttack2();
    checkColliderHammerAttack();
  }

  TimerAnimationAttack--;

  if (TimerAnimationAttack <= 0){

    TimerAnimationAttack = MAX_TIMER;
    isAttacking = 0;

  }

}

void JudgeSendSentence() {
  drawjudgeShockWave();
}

void BanditWalk(UBYTE numero, UBYTE posX, UBYTE posY) {
  bandits[numero].isWalkingBandit = 1;


  if(bandits[numero].isAttackingBandit == 0){
    if(bandits[numero].timerAnimationWalkBandit >= MAX_TIMER/2){
      drawBanditWalk1(numero+1, posX, posY);
    } else if(bandits[numero].timerAnimationWalkBandit >= 0 && bandits[numero].timerAnimationWalkBandit < MAX_TIMER/2){
      drawBanditWalk2(numero+1, posX, posY);
    }

    bandits[numero].timerAnimationWalkBandit--;

    if(bandits[numero].timerAnimationWalkBandit <= 0){
      bandits[numero].timerAnimationWalkBandit = MAX_TIMER;

    }
  }

}

void BanditAttack(UBYTE numero, UBYTE posX, UBYTE posY) {
  bandits[numero].isAttackingBandit = 1;
  bandits[numero].isWalkingBandit = 0;

  if(bandits[numero].timerAnimationAttackBandit >= MAX_TIMER/2){
    drawBanditAttack1(numero+1, posX, posY);
  }  else if(bandits[numero].timerAnimationAttackBandit >= 0 && bandits[numero].timerAnimationAttackBandit < MAX_TIMER/2){
    drawBanditAttack2(numero+1, posX, posY);
    if(isHurt == 0){
     isHurt = 1;
   }

}

 bandits[numero].timerAnimationAttackBandit--;

 if (bandits[numero].timerAnimationAttackBandit <= 0){

  bandits[numero].timerAnimationAttackBandit = MAX_TIMER;

}
}

void BanditDie(UBYTE numero, UBYTE posX, UBYTE posY) {
  bandits[numero].isDyingBandit = 1;
  bandits[numero].isWalkingBandit = 0;
  bandits[numero].isAttackingBandit = 0;

  drawBanditDead(numero+1, posX, posY);

  bandits[numero].timerAnimationDyingBandit--;

  if(bandits[numero].timerAnimationDyingBandit <= 0){
    bandits[numero].timerAnimationDyingBandit = MAX_TIMER;
    drawBanditDead(numero+1, -5, -5);
    bandits[numero].pv = 0;

  }

}


/***
 * MOVE FUNCTIONS IMPL
*/
void moveJudge() {

  if(joypad()){
    if(joypad() & J_RIGHT){
      if(isAttacking == 0){
        posXJudge += JudgeSpeed;
      }

      checkCollideBorder(posXJudge, posYJudge);
      JudgeWalk();
    }
    if(joypad() & J_UP){

      if(isAttacking == 0){
        posYJudge -= JudgeSpeed;
        checkCollideBorder(posXJudge, posYJudge);
        JudgeWalk();
      }

    }
    if(joypad() & J_DOWN){
      if(isAttacking == 0){
        posYJudge += JudgeSpeed;
      }
      checkCollideBorder(posXJudge, posYJudge);
      JudgeWalk();
    }

    if(joypad() & J_LEFT){
      if(isAttacking == 0){
        posXJudge -= JudgeSpeed;
      }

      checkCollideBorder(posXJudge, posYJudge);
      JudgeWalk();
    }
    if(joypad() & J_A){
        isAttacking = 1;
    }
    if(joypad() & J_B){
      if(shockSend == 0){
        shockSend = 1;
        isAttacking = 1;
        sound_atk();
        JudgeSendSentence();
      }

    }
  }else{
    isWalking = 0;
  }

  if(isHurt == 1){
    JudgeDoHurt();
  }


}

void moveShockWave() {

  if(gameIsLaunch == 1){
      posXShock+= 2;
  }

  move_sprite(32, posXShock ,posYShock);

  checkCollideShockAttack();

}

void moveBandits() {

  for(i = 0; i < NB_BANDIT; i++){
    if(bandits[i].isNotHere == 1){
      BanditWalk(bandits[i].t-1,-50, -50);
    }
    else if(bandits[i].pv == 1){

      if(bandits[i].isDyingBandit == 0){
        if(bandits[i].isWalkingBandit == 1){
          BanditWalk(bandits[i].t-1, bandits[i].PosX, bandits[i].PosY);
          switch(bandits[i].comportement){
            case BANDIT_RUN:
            bandits[i].PosX -= bandits[i].speed;
            break;

            case BANDIT_FOLLOW:

            bandits[i].PosX -= bandits[i].speed;
            break;
          }
        }


        if(bandits[i].PosX < borderLeft){
          bandits[i].pv = 0;
          if(gameIsLaunch == 1){
            score_villager--;
          }
          
        }
        if(bandits[i].PosX < posXJudge+8 && bandits[i].PosX > posXJudge && bandits[i].PosY < posYJudge+24  && bandits[i].PosY > posYJudge+8){
          BanditAttack(bandits[i].t-1, bandits[i].PosX, bandits[i].PosY);


        }else{
          bandits[i].isWalkingBandit = 1;
          bandits[i].isAttackingBandit = 0;
        }


      } else {
        BanditDie(bandits[i].t-1, bandits[i].PosX, bandits[i].PosY);
      }
    }

  }


}

void scrollCity() {

  if(TimerScrollBKG % 2 == 0){
    scroll_bkg(1,0);
    posXJudge--;
    for(i = 0; i < nbBanditToDraw; i++){
      bandits[i].PosX--;
    }

    checkCollideBorder(posXJudge, posYJudge);
  }

  TimerScrollBKG--;

  if (TimerScrollBKG <= 0){

    TimerScrollBKG = MAX_TIMER_SCROLLBKG;

  }
}


/***
 * CHORE FUNCTIONS IMPL
*/
void clearBkgAndWin() {
  DISPLAY_OFF;

  for(i = 0; i < 30; i++){
    for(j = 0; j < 30; j++){
      set_bkg_tiles(i, j, 1, 1, 0x0E);
    }
  }

  for(i = 0; i < 30; i++){
    for(j = 0; j < 30; j++){
    set_win_tiles(i, j, 1, 1, 0x0E);
    }
  }

  move_bkg(0,0);
  DISPLAY_ON;
}

void generateBanditsST() {

  bandits[2].isNotHere = 0;
  bandits[1].isNotHere = 0;
  bandits[0].isNotHere = 0;

  for(i = 0; i < NB_BANDIT; i++){
    if(bandits[i].pv == 0){
      bandits[i].pv = 1;
      bandits[i].isDyingBandit = 0;
      bandits[i].PosX = borderRight + 50 + (rand() % 50);
      bandits[i].timerAnimationDyingBandit = MAX_TIMER;
      bandits[i].timerAnimationAttackBandit = MAX_TIMER;
      bandits[i].timerAnimationWalkBandit = MAX_TIMER;
      do{
        bandits[i].PosY = rand() % SCREENHEIGHT+40;
      }while(bandits[i].PosY > SCREENHEIGHT+40 || bandits[i].PosY < (borderTop*2)+48);

      if(i > 0){
        do{
          bandits[i].PosY = rand() % SCREENHEIGHT+40;
        }while(bandits[i].PosY == bandits[i-1].PosY || bandits[i].PosY > SCREENHEIGHT+40 || bandits[i].PosY < (borderTop*2)+48);
      }

      bandits[i].t = i+1;

      bandits[i].comportement = 1;

      bandits[i].isWalkingBandit = 1;

      pickComportement = rand() % 100;
      if(pickComportement > 20){
        bandits[i].speed = 1;
      }else{
        bandits[i].speed = 2;
      }

    }

  }
}

void generateBandits() {


  if(score_bandit <= 5){
    bandits[2].isNotHere = 1;
    bandits[1].isNotHere = 1;
    bandits[0].isNotHere = 0;
  } else if(score_bandit <= 10){
    bandits[2].isNotHere = 1;
    bandits[1].isNotHere = 0;
    bandits[0].isNotHere = 0;
  } else if (score_bandit > 15){
    bandits[2].isNotHere = 0;
    bandits[1].isNotHere = 0;
    bandits[0].isNotHere = 0;

  }



  for(i = 0; i < NB_BANDIT; i++){
    if(bandits[i].pv == 0){
      bandits[i].pv = 1;
      bandits[i].isDyingBandit = 0;
      bandits[i].PosX = borderRight +16;
      bandits[i].timerAnimationDyingBandit = MAX_TIMER;
      bandits[i].timerAnimationAttackBandit = MAX_TIMER;
      bandits[i].timerAnimationWalkBandit = MAX_TIMER;
      do{
        bandits[i].PosY = rand() % borderBottom;
      }while(bandits[i].PosY > borderBottom+16 || bandits[i].PosY < borderTop+16);

      if(i > 0){
        do{
          bandits[i].PosY = rand() % borderBottom;
        }while(bandits[i].PosY == bandits[i-1].PosY || bandits[i].PosY > borderBottom+16 || bandits[i].PosY < borderTop+16);
      }

      bandits[i].t = i+1;

      pickComportement = rand() % 100;
      if(pickComportement > 99){
        bandits[i].comportement = 1;
      }else{
        bandits[i].comportement = 2;
      }
      bandits[i].isWalkingBandit = 1;

    }

  }

}

void calculateScoreVillager(){

  if(score_villager == 10){
    score_villager_digitDeux = 1;
    score_villager_digitUn = 0;
  } else if (score_villager == 9){
    score_villager_digitDeux = 0;
    score_villager_digitUn = 9;
  } else if (score_villager < 9){
    score_villager_digitUn = score_villager;
  } 

  if (score_villager <= 0){
    isGameOver = 1;
  }


}

void checkBanditScore() {
  if(score_bandit_digitUn > 9){
    score_bandit_digitUn = 0;
    score_bandit_digitDeux++;
  }
  if(score_bandit_digitDeux > 9){
    score_bandit_digitDeux = 0;
    score_bandit_digitTrois++;
  }
  if(score_bandit_digitTrois > 9){
    score_bandit_digitUn = 0;
    score_bandit_digitDeux = 0;
    score_bandit_digitTrois = 0;
  }
}


int main() {

  initialSetup();

  showBootScreens();

  //Game Loop
  while(1)
  {

    NR10_REG = 0x34U;

    clearBkgAndWin();
    HIDE_SPRITES;
    HIDE_WIN;
    HIDE_BKG;
    DISPLAY_OFF;

    initBandits();

    //Start Screen Loop
    showStartScreen();

    //Game Screen Loop
    showGameScreen();

    //showGameOver();
    showScoreScreen();

  }
  return 0;
}
