async function sendMove(from, to) {

    const response = await fetch("http://localhost:8000/move", {
        method: "POST",

        headers: {
            "Content-Type": "application/json"
        },

        body: JSON.stringify({
            from: from,
            to: to
        })
    });

    const result = await response.json();

    console.log(result);

    return result;
}

// Test
sendMove(12, 28);