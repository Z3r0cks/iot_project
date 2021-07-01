// $TITLE, $SCRIPT, $BODY
const char playerPage[] = R"=====(<!DOCTYPE html><html lang="de"><head><meta charset="UTF-8"><meta http-equiv="X-UA-Compatible" content="IE=edge"><meta name="viewport" content="width=device-width, initial-scale=1.0"><title>$TITLE</title><link rel="stylesheet" href="style.css">$SCRIPT</head><body class="$CLASS">$BODY</body></html>)=====";

const char playerIndex[] = R"=====(
 <div class="wrapper"><div class="headerWrapper indexheaderWrapper"><p class="indexHeader">Willkommen Player!</p><p>Gib den Code des <br>Gamemasters ein </p></div><input type="text" name="gamecode-input" id="gamecode-input" placeholder="code..."><button class="btn" id="enter-game">Spiel beitreten</button></div>
)=====";

const char playerSolution[] = R"=====(
<div class="wrapper">
    <div class="headerWrapper solutionHeaderWrapper">
        <p class="header"></p>
    </div>
    <div class="contentWrapper">
        <p id="feedback"></p>
    </div>
    <div class="contentWrapper">
        <p id="solution"></p>
    </div>
    <div class="contentWrapper">
        <p id="wait-info"></p>
    </div>
</div>
)=====";

const char playerQuestion[] = R"=====(
<div class="wrapper">
    <div class="headerWrapper questionHeaderWrapper">
        <p class="header questionHeader" id="question">Was macht den Schwarzwald so
            bienenfreundlich?</p>
    </div>
    <div id="answers">
        <div class="answerWrapperBlock">
            <div class="oneAnswerWrapper">
                <p class="questionTitle margin-right">Antwort A</p>
                <div class="answerWrapper margin-right">
                    <span id="answerA">Die vielen Höhenmeter, die Bienen gerne haben.</span>
                </div>
            </div>
            <div class="oneAnswerWrapper">
                <p class="margin-left questionTitle">Antwort B</p>
                <div class="answerWrapper margin-left">
                    <span id="answerB">Die gute Luftqualität im Schwarzwald.</span>
                </div>
            </div>
        </div>
        <div class="answerWrapperBlock">
            <div class="oneAnswerWrapper">
                <p class="margin-right questionTitle">Antwort C</p>
                <div class="answerWrapper margin-right">
                    <span id="answerC">Das viele Löwenzahn, das im Schwarzwald wächst.</span>
                </div>
            </div>
            <div class="oneAnswerWrapper">
                <p class="margin-left questionTitle">Antwort D</p>
                <div class="answerWrapper margin-left">
                    <span id="answerD">Der häufige und viele Schneefall im Schwarzwald.</span>
                </div>
            </div>
        </div>
    </div>
    <button id="login-answer">Antwort einloggen</button>
</div>
)=====";

const char playerVictory[] = R"=====(
<h3>Du hast gewonnen!</h3>
)=====";

const char playerDefeat[] = R"=====(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8"><meta http-equiv="X-UA-Compatible" content="IE=edge"><meta name="viewport" content="width=device-width, initial-scale=1.0"><link rel="stylesheet" href="style.css"><title>Niederlage</title></head><body><div class="wrapper"><div class="headerWrapper"><p class="header">Niederlage</h3></div></div></body></html>
)=====";

const char playerError[] = R"=====(
<h3>Ups!</h3>
<p>Etwas ist schief gelaufen.</p>
)=====";