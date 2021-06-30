// $TITLE, $SCRIPT, $BODY
const char masterPage[] = R"=====(<!DOCTYPE html><html lang="de"><head><meta charset="UTF-8"><meta http-equiv="X-UA-Compatible" content="IE=edge"><meta name="viewport" content="width=device-width, initial-scale=1.0"><title>$TITLE</title><link rel="stylesheet" href="style.css">$SCRIPT</head><body>$BODY</body></html>)=====";

const char masterIndex[] = R"=====(
<h3>Code</h3>
<h1 id="gamecode">----</h1>
)=====";

const char masterQuestions[] = R"=====(<div class="scoreWrapper">
<div class="score">GM <span id="scoreM">$SCORE_PLAYER</span> : <span id="scoreP">$SCORE_MASTER</span> P</div>
</div>
<div class="headerWrapper">
    <h3 class="header">Wähle eine Kategorie aus</h1>
</div>
<div class="catWrapper">
    <a class="cat" href="#cat1">Flora</a>
    <a class="cat" href="#cat2">Fauna</a>
</div>
<div class="catWrapper">
    <a class="cat" href="#cat3">FuWa Things</a>
    <a class="cat" href="#cat4">Berge und Flüsse</a>
</div>
<div id="cat1"></div>
<div id="cat2"></div>
<div id="cat3"></div>
<div id="cat4"></div>
)=====";

const char masterSolution[] = R"=====(
<p id="solution"></p>
<p id="wait-info"></p>
)=====";

const char masterVictory[] = R"=====(
<h3>Sieg</h3>
)=====";

const char masterDefeat[] = R"=====(
<h3>Niederlage</h3>
)=====";

const char masterError[] = R"=====(
<h3>Ups!</h3>
<p>Etwas ist schief gelaufen.<p>
)=====";