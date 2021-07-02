// $TITLE, $SCRIPT, $BODY
const char playerPage[] = R"=====(<!DOCTYPE html><html lang="de"><head><meta charset="UTF-8"><meta http-equiv="X-UA-Compatible" content="IE=edge"><meta name="viewport" content="width=device-width, initial-scale=1.0"><title>$TITLE</title><link rel="stylesheet" href="style.css">$SCRIPT</head><body class="$CLASS">$BODY</body></html>)=====";

const char playerIndex[] = R"=====(
 <div class="wrapper"><div class="headerWrapper indexheaderWrapper"><p class="indexHeader">Willkommen Player!</p><p>Gib den Code des <br>Gamemasters ein </p></div><input type="text" name="gamecode-input" id="gamecode-input" placeholder="code..."><button class="btn" id="enter-game">Spiel beitreten</button></div>
)=====";

const char playerSolution[] = R"=====(
 <div class="wrapper"><div class="scoreWrapper"><div class="score">P&nbsp;<span id="scoreP">$SCORE_PLAYER</span>&nbsp;:&nbsp;<span id="scoreM">$SCORE_MASTER</span>&nbsp;GM</div></div><div class="headerWrapper solutionHeaderWrapper"><p class="header"></p></div><div class="contentWrapper"><p id="feedback"></p></div><div class="contentWrapper"><p id="solution"></p></div><div class="contentWrapper"><p id="wait-info">Der Gamemaster wählt eine Fage aus. Bitte warten.</p></div></div>
)=====";

const char playerQuestion[] = R"=====(
<div class="wrapper"><div class="scoreWrapper"><div class="score">P&nbsp;<span id="scoreP">$SCORE_PLAYER</span>&nbsp;:&nbsp;<span id="scoreM">$SCORE_MASTER</span>&nbsp;GM</div></div><div class="headerWrapper questionHeaderWrapper"><p class="header questionHeader" id="question"></p></div><div id="answers"><div class="answerWrapperBlock"><div class="oneAnswerWrapper"><p class="questionTitle margin-right">Antwort A</p><button class="answerWrapper margin-right"><span id="answerA"></span></button></div><div class="oneAnswerWrapper"><p class="margin-left questionTitle">Antwort B</p><button class="answerWrapper margin-left"><span id="answerB"></span></button></div></div><div class="answerWrapperBlock"><div class="oneAnswerWrapper margin-bottom"><p class="margin-right questionTitle">Antwort C</p><button class="answerWrapper margin-right"><span id="answerC"></span></button></div><div class="oneAnswerWrapper margin-bottom"><p class="margin-left questionTitle">Antwort D</p><button class="answerWrapper margin-left"><span id="answerD"></span></button></div></div></div><button class="btn" id="login-answer">Antwort einloggen</button></div>
)=====";

const char playerVictory[] = R"=====(
 <div class="wrapper"><div class="headerWrapper"><h3 class="header">Gewonnen</h3><p>Das Quiz beendet sich in 10 Sekunden</p></div></div>
)=====";

const char playerDefeat[] = R"=====(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8"><meta http-equiv="X-UA-Compatible" content="IE=edge"><meta name="viewport" content="width=device-width, initial-scale=1.0"><link rel="stylesheet" href="style.css"><title>Niederlage</title></head><body><div class="wrapper"><div class="headerWrapper"><p class="header">Niederlage</h3></div></div></body></html>
)=====";

const char playerError[] = R"=====(
 <div class="wrapper"><div class="headerWrapper"><h3>Ups!</h3><p>Etwas ist schief gelaufen.</p></div></div>
)=====";