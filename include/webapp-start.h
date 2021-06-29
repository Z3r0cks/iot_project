const char webappStart[] = R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP WebApp</title>
    <style>
        body {
            background-color: #436436;
        }

        h2 {
            margin: 30px 0 8px 0;
            padding: 0;
        }

        .question {
            font-size: 18px;
            margin-bottom: 1px;
            padding-left: 8px;
        }

        .wrapper {
            max-width: 450px;
            margin: 0 auto;
            display: flex;
            flex-direction: column;
            padding: 0 20px;
        }

        .text {
            color: #fefefe;
            font-family: "Roboto", sans-serif;
        }

        .textarea,
        input {
            border-radius: 24px;
            background-color: #7a8f72;
            border: none;
            color: #3d3d3d;
            padding: 5px 20px;
            margin-bottom: 10px;
        }

        .input {
            height: 35px;
        }

        .btn {
            height: 100px;
            width: 100px;
            border-radius: 50px;
            border: none;
            margin: 30px auto 0 auto;
            background-color: #7a8f72;
            color: white;
            font-size: 18px;
        }

        .btn:hover {
            background-color: #495544;
        }
        #error{
            display: none;
            background: rgba(255, 94, 0, 0.5);
            color: rgb(230, 230, 230);
            padding: 0.2em;
        }
        #success{
            display: none;
            border:2px solid #fefefe;
            padding: 0.5em;
        }
    </style>
</head>

<body>

    <form class="wrapper" id="form" type="GET">
        <h1 class="text">DIE NATUR FORDERT DICH HERAUS</h1>
        <h2 class="text">Neue Frage erstellen</h2>

        <label class="text question" for="question">Frage</label>
        <textarea class="textarea m-b" name="" id="question cols=" 30" rows="10" onkeyup="checkAllInputs()"
            onfocus="inputActiveColor(this)" onblur="inputPassiveColor(this)"></textarea>

        <label class="text question" for="answerA">Antwort A</label>
        <input class="input m-b" id="answerA" type="text" name="answerA" id="answerA" onkeyup="checkAllInputs()"
            onfocus="inputActiveColor(this)" onblur="inputPassiveColor(this)">

        <label class="text question" for="answerB">Antwort B</label>
        <input class="input m-b" type="text" name="answerB" id="answerB" onkeyup="checkAllInputs()"
            onfocus="inputActiveColor(this)" onblur="inputPassiveColor(this)">

        <label class="text question" for="answerC">Antwort C</label>
        <input class="input m-b" type="text" name="answerC" id="answerC" onkeyup="checkAllInputs()"
            onfocus="inputActiveColor(this)" onblur="inputPassiveColor(this)">

        <label class="text question" for="answerD">Antwort D</label>
        <input class="input m-b" type="text" name="answerD" id="answerD" onkeyup="checkAllInputs()"
            onfocus="inputActiveColor(this)" onblur="inputPassiveColor(this)">

        <!-- <input class="btn" type="button" onclick="validateForm()" name="submit" value="Frage einreichen"> -->
        <p id="error" class="text">Alle Felder müssen ausgefüllt sein.</p>
        <p id="success" class="text">Die Frage wurde erfolgreich eingereicht. Sie wird geprüft und ggf. in das Quiz aufgenommen.</p>
        <button class="btn" type="button" name="submit" onclick="formNotActive()">Frage einreichen</button>
    </form>
)=====";