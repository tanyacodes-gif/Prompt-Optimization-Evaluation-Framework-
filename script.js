const addPromptBtn = document.getElementById("addPromptBtn");
const runBtn = document.getElementById("runBtn");
const promptsDiv = document.getElementById("prompts");
const resultsDiv = document.getElementById("results");

addPromptBtn.addEventListener("click", addPrompt);
runBtn.addEventListener("click", runEvaluation);

function addPrompt() {
  const textarea = document.createElement("textarea");
  textarea.placeholder = "New prompt version";
  promptsDiv.appendChild(textarea);
}

function runEvaluation() {
  resultsDiv.innerHTML = "";

  const promptTextareas = document.querySelectorAll("#prompts textarea");
  let bestScore = 0;

  promptTextareas.forEach((prompt, index) => {
    const accuracy = randomScore();
    const tone = randomScore();
    const completeness = randomScore();
    const finalScore = accuracy + tone + completeness;

    const card = document.createElement("div");
    card.classList.add("card");

    if (finalScore > bestScore) {
      bestScore = finalScore;
      document.querySelectorAll(".card").forEach(c => c.classList.remove("best"));
      card.classList.add("best");
    }

    card.innerHTML = `
      <h3>Prompt ${index + 1}</h3>
      <p><strong>Prompt:</strong> ${prompt.value || "—"}</p>
      <p><strong>Response:</strong> (Model response will appear here)</p>
      <p>Accuracy: ${accuracy}</p>
      <p>Tone: ${tone}</p>
      <p>Completeness: ${completeness}</p>
      <p><strong>Final Score: ${finalScore}</strong></p>
    `;
    resultsDiv.appendChild(card);
  });
}
function randomScore() {
  return Math.floor(Math.random() * 5) + 1;
}
fetch("http://localhost:8080/evaluate", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({
      task_input: "Sample task",
      prompts: ["Prompt 1", "Prompt 2"]
    })
  })
  .then(res => res.json())
  .then(data => console.log(data));