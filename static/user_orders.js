console.log("Hello from data.js");

var trading_form = document.getElementById('trade_form');
// form.onsubmit = function() {
//     form.target = '_self';
// };
const
    buy_btn = document.getElementById('buy_btn'),
    sell_btn = document.getElementById('sell_btn');
buy_btn.addEventListener('click', sendBuyOrder);
sell_btn.addEventListener('click', sendSellOrder);

// loginForm.addEventListener("submit", );

async function sendBuyOrder() {
    const formData = new FormData(trading_form);

    console.log("buy order sent", formData.get("trade_amount"));

    response = await fetch("http://localhost:8081/api/trade", {
        method: "POST",
        body: formData,
        headers: {
            "X-Csrftoken": formData.get("csrf_token"),
            "Content-type": "application/json; charset=UTF-8",
            'Accept': 'application/json, text/plain, text/html, */*',
        }
    })
    .then((response) => response.json())
    .then((json) => {
        console.log(json);
    });
}

async function sendSellOrder() {
    const formData = new FormData(trading_form);
    console.log("sell order sent",  formData.get("trade_amount"));    
}