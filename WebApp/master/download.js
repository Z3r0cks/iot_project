const raw = "$DOWNLOAD";
if (raw) {
    const data = decodeURIComponent(raw);
    const file = new Blob([data], { type: "application/json" });
    const url = URL.createObjectURL(file);
    const a = document.createElement("a");
    a.href = url;
    a.download = "questions.json";
    a.innerText = "questions.json";
    document.body.appendChild(a);
    const questions = JSON.parse(data);
    for (const quest of questions) {
        const questDiv = document.createElement("div");
        questDiv.innerHTML = `
    <div>${quest.question}</div>
    <div>
        <div>${quest.a}</div>
        <div>${quest.b}</div>
        <div>${quest.c}</div>
        <div>${quest.d}</div>
    </div>`;
        document.body.appendChild(questDiv);
    }
}