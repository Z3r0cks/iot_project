const questions = $QUESTIONS;
for (const key in questions) {
    const quest = questions[key];
    const div = document.createElement("button");
    div.classList.add("question");
    div.id = key;
    
    div.innerHTML = quest.text;
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