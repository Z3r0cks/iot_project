const questions = $QUESTIONS;
document.querySelector("#solution").innerHTML = questions["$ID"].text;
document.querySelector("#wait-info").innerHTML = questions["$ID"].explanation;
const interval = setInterval(function () {
    const xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            if (this.responseText.trim() == "$VALIDATION") {
                clearInterval(interval);
                window.location.reload();
            }
        }
    };
    xhttp.open("GET", "validate-answer", true);
    xhttp.send();
}, 1000);