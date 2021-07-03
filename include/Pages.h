// $TITLE, $SCRIPT, $BODY
const char masterPage[] = R"=====(
<!DOCTYPE html><html lang="de"><head><meta charset="UTF-8"><meta http-equiv="X-UA-Compatible" content="IE=edge"><meta name="viewport" content="width=device-width, initial-scale=1.0"><title>$TITLE</title><link rel="stylesheet" href="$STYLE">$SCRIPT</head><body class="$CLASS">$BODY</body></html>
)=====";

const char masterIndex[] = R"=====(
 <div class="wrapper"><div class="headerWrapper"><h3 class="">Code</h3><h1 class="" id="gamecode">----</h1></div><a class="btn" href="/suggest-question">Frage vorschlagen</a></div>
)=====";

const char masterQuestions[] = R"=====(
 <div class="wrapper"><div class="scoreWrapper"><div class="score">GM&nbsp;<span id="scoreM">$SCORE_MASTER</span>&nbsp;:&nbsp;<span id="scoreP">$SCORE_PLAYER</span>&nbsp;P</div></div><div class="headerWrapper questionHeader"><h3 class="header">Wähle eine Kategorie aus</h1></div><div class="categories"><div class="catBlockWrapper"><div class="catWrapper"><a class="cat" href="#cat1">Flora</a></div><div class="catWrapper"><a class="cat" href="#cat2">Fauna</a></div></div><div class="catBlockWrapper"><div class="catWrapper"><a class="cat" href="#cat3">FuWa Things</a></div><div class="catWrapper"><a class="cat" href="#cat4">Berge und Flüsse</a></div></div></div><div id="cat1"><div class="catTitleWrapper"><h3 class="catText">Flora</h3></div></div><div id="cat2"><div class="catTitleWrapper"><h3 class="catText">Fauna</h3></div></div><div id="cat3"><div class="catTitleWrapper"><h3 class="catText">FuWa Things</h3></div></div><div id="cat4"><div class="catTitleWrapper"><h3 class="catText">Berge und Flüsse</h3></div></div></div>
)=====";

const char masterSolution[] = R"=====(
 <div class="wrapper"><div class="scoreWrapper"><div class="score">GM&nbsp;<span id="scoreM">$SCORE_MASTER</span>&nbsp;:&nbsp;<span id="scoreP">$SCORE_PLAYER</span>&nbsp;P</div></div><div class="headerWrapper solutionWrapper"><p class="solutionText" id="solution"></p></div><div class="headerWrapper solutionWrapper2"><p class="solutionText" id="wait-info"></p></div></div>
)=====";

const char masterVictory[] = R"=====(
 <div class="wrapper"><div class="headerWrapper"><h3 class="header siegHeader">Sieg </h3><p>Das Quiz beendet sich in 10 Sekunden</p></div></div>
)=====";

const char masterDefeat[] = R"=====(
<div class="wrapper"><div class="headerWrapper"><h3 class="header siegHeader">Leider verloren</h3><p>Das Quiz beendet sich in 10 Sekunden</p></div></div>
)=====";

const char formIndex[] = R"=====(
<form class="wrapper" id="form"><h1 class="text">DIE NATUR FORDERT DICH HERAUS</h1><h2 class="text">Neue Frage erstellen</h2><label class="text question" for="question">Frage</label><textarea class="textarea m-b" name="" id="question" cols=" 30" rows="10"></textarea><label class="text question" for="a">Richtige Antwort</label><input class="input m-b" id="answerA" type="text" id="answerA"><label class="text question" for="b">Falsche Antwort</label><input class="input m-b" type="text" id="answerB"><label class="text question" for="c">Falsche Antwort</label><input class="input m-b" type="text" id="answerC"><label class="text question" for="d">Falsche Antwort</label><input class="input m-b" type="text" id="answerD"><p id="error" class="text hidden">Alle Felder müssen ausgefüllt sein.</p><p id="success" class="text hidden">Die Frage wurde erfolgreich eingereicht. Sie wird geprüft und ggf. in das Quiz aufgenommen.</p><button class="btn" type="button" id="submit">Frage einreichen</button><a class="btn" href="/">Zurück</a></form>
)=====";