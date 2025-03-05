const express = require('express');
const app = express();
const port = process.env.PORT || 3000;

app.get("/", (req, res) => {
    res.setHeader("Content-Type", "text/html");
    res.send(`<iframe style="border-radius:12px" src="https://open.spotify.com/embed/playlist/5RbOU0xpR7vmUYLlrnYnQJ?utm_source=generator" width="100%" height="352" frameBorder="0" allowfullscreen="" allow="autoplay; clipboard-write; encrypted-media; fullscreen; picture-in-picture" loading="lazy"></iframe>`);
});

app.listen(port, () => {
    console.log(`server is listening at localhost:${port}`);
});