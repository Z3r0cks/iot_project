const questions = $QUESTIONS;
for (const key in questions) {
    const q = questions[key];
    const div = document.createElement("div");
    div.classList.add("question");
    div.id = key;
    const text = document.createElement("div");
    text.innerHTML = q.text;
    div.appendChild(text);
    for (const answer of q.answers) {
        const a = document.createElement("a");
        a.innerHTML = answer;
        div.appendChild(a);
    }
    function clickHandler() {
        const xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                if (this.responseText.trim() == "$VALIDATION") {
                    div.removeEventListener("click", clickHandler);
                    window.location.reload();
                }
            }
        };
        xhttp.open("GET", "choose-question?question=" + key, true);
        xhttp.send();
    }
    div.addEventListener("click", clickHandler);
    document.querySelector("#cat" + questions[key].category).appendChild(div);
}