// $TITLE, $SCRIPT, $BODY
const char masterPage[] = R"=====(<!DOCTYPE html><html lang="de"><head><meta charset="UTF-8"><meta http-equiv="X-UA-Compatible" content="IE=edge"><meta name="viewport" content="width=device-width, initial-scale=1.0"><title>$TITLE</title><link rel="stylesheet" href="style.css">$SCRIPT</head><body class="$CLASS">$BODY</body></html>)=====";

const char masterIndex[] = R"=====(
 <div class="wrapper"><div class="headerWrapper"><h3 class="">Code</h3><h1 class="" id="gamecode">----</h1></div></div>
)=====";

const char masterQuestions[] = R"=====(
<div class="wrapper"><div class="scoreWrapper"><div class="score">GM <span id="scoreM">$SCORE_MASTER</span>: <span id="scoreP">$SCORE_PLAYER</span>P </div></div><div class="headerWrapper questionHeader"><h3 class="header">Wähle eine Kategorie aus</h1></div><div class="categories"><div class="catBlockWrapper"><div class="catWrapper"><a class="cat" href="#cat1">Flora</a></div><div class="catWrapper"><a class="cat" href="#cat2">Fauna</a></div></div><div class="catBlockWrapper"><div class="catWrapper"><a class="cat" href="#cat3">FuWa Things</a></div><div class="catWrapper"><a class="cat" href="#cat4">Berge und Flüsse</a></div></div></div><div id="cat1"><div class="catTitleWrapper"><h3 class="catText">Flora</h3></div></div><div id="cat2"><div class="catTitleWrapper"><h3 class="catText">Fauna</h3></div></div><div id="cat3"><div class="catTitleWrapper"><h3 class="catText">FuWa Things</h3></div></div><div id="cat4"><div class="catTitleWrapper"><h3 class="catText">Berge und Flüsse</h3></div></div></div>
)=====";

const char masterSolution[] = R"=====(
 <div class="wrapper"><div class="scoreWrapper"><div class="score">GM <span id="scoreM">$SCORE_MASTER</span>: <span id="scoreP">$SCORE_PLAYER</span>P </div></div><div class="headerWrapper solutionWrapper"><p class="solutionText" id="solution">Lorem, ipsum dolor sit amet consectetur adipisicing elit. Dicta enim ex explicabo. A deserunt beatae velit rem optio, sequi esse vitae omnis facilis. Aliquam sed, esse culpa molestiae asperiores inventore!</p></div><div class="headerWrapper solutionWrapper2"><p class="solutionText" id="wait-info">Warte bis der Gamemaster eine neue Frage ausgewählt hat</p></div>
)=====";

const char masterVictory[] = R"=====(
 <div class="wrapper"><div class="headerWrapper"><h3 class="header siegHeader">Sieg </h3><p>Das Quiz geht in 10 Sekunden in den SleepModus</p></div></div>
)=====";

const char masterDefeat[] = R"=====(
 <div class="wrapper"><div class="headerWrapper"><h3 class="header">Niederlage</h3></div></div>
)=====";

const char masterError[] = R"=====(
 <div class="wrapper"><div class="headerWrapper"><h3>Ups!</h3><p>Etwas ist schief gelaufen.</p></div></div>
)=====";