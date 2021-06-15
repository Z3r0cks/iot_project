const char webappStart[] = R"=====(
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP WebApp</title>
    <link rel="stylesheet" href="css/styles.css">
</head>

<body>
    <h1>Naturquiz</h1>

    <form id="form" type="GET">
        <input type="text" name="question" id="question" value="Ich heiße Klaus">
        <label for="question">Frage</label>

        <input type="text" name="answerA" id="answerA" value="ä Ä ö Ö ü Ü">
        <label for="answerA">Antwort A</label>
        <input type="text" name="answerB" id="answerB">
        <label for="answerB">Antwort B</label>
        <input type="text" name="answerC" id="answerC">
        <label for="answerC">Antwort C</label>
        <input type="text" name="answerD" id="answerD">
        <label for="answerD">Antwort D</label>

        <input type="button" onclick="validateForm(); window.location.reload();" name="submit" value="Abschicken">
    </form>
)=====";