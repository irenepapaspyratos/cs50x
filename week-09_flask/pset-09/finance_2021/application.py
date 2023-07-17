import os
import datetime

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/", methods=["GET", "POST"])
@login_required
def index():
    """Show portfolio of stocks"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure amount to increase is positive number
        if not request.form.get("cash") or request.form.get("cash") == "0" or request.form.get("cash").isdigit() == False:
            return apology("must provide a positive number of whole dollars to increase", 403)

        # Get values needed
        u_id = session["user_id"]
        amount = request.form.get("cash")
        money = db.execute('SELECT cash FROM users WHERE id = ?', u_id)
        new_money = float(money[0]['cash']) + (int(amount))
        dt = datetime.datetime.utcnow()
        dt_date = dt.strftime("%Y-%m-%d")
        dt_time = dt.strftime("%H:%M:%S")
        curr_action = db.execute('SELECT id FROM actions WHERE action = ?', 'deposit')[0]['id']

        # Insertion to transactions_utc
        db.execute('INSERT INTO transactions_utc (user_id, symbol, date, time, action_id, shares, price) VALUES (?, ?, ?, ?, ?, ?, ?)',
                   u_id, "$", dt_date, dt_time, curr_action, int(amount), usd(int(amount)))

        # Update user's cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_money, u_id)

        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        # Get values needed next
        u_id = session["user_id"]
        dt = datetime.datetime.utcnow()
        dt_date = dt.strftime("%Y-%m-%d")
        dt_time = dt.strftime("%H:%M:%S")

        buy_id = db.execute('SELECT id FROM actions WHERE action = ?', 'buy')[0]['id']
        stocks = db.execute('SELECT symbol, SUM(CASE WHEN action_id = ? THEN shares ELSE shares * -1 END) AS sum FROM transactions_utc '
                            'WHERE user_id = ? GROUP BY symbol HAVING sum > 0', buy_id, u_id)

        sum_v = 0
        for row in stocks:
            cur = lookup(row['symbol'])
            cur_price = round(cur['price'], 2)
            row['name'] = cur['name']
            row['price'] = usd(cur_price)
            row['shares'] = row['sum']
            row['value'] = usd(cur_price * int(row['sum']))

            sum_v += (cur_price * int(row['sum']))

        money = db.execute('SELECT cash FROM users WHERE id = ?', u_id)
        cash = usd(money[0]['cash'])
        new_money = usd(float(money[0]['cash']) + sum_v)
        p = usd(1.00)
        stocks.append({'name': 'Cash', 'symbol': '$', 'price': p, 'shares': round(money[0]['cash'], 2),
                      'value': usd(money[0]['cash'])})

        return render_template("/index.html", stocks=stocks, complete=new_money)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        # Ensure symbol exists database of iexapis.com
        cur_symbol = request.form.get("symbol")
        if lookup(cur_symbol) == None:
            return apology("requested symbol not found in database", 400)

        # Ensure number of shares to sell is positive integer
        if not request.form.get("shares") or request.form.get("shares") == "0" or request.form.get("shares").isdigit() == False:
            return apology("must provide a positive number of whole shares to buy", 400)

        # Get values needed next
        u_id = session["user_id"]
        cur = lookup(cur_symbol)
        shares = int(request.form.get("shares"))
        money = db.execute('SELECT cash FROM users WHERE id = ?', u_id)
        new_money = float(money[0]['cash']) - (round(cur["price"], 2) * shares)

        # Ensure user has enough cash to buy
        if new_money < 0:
            return apology("must provide enough cash for this order", 403)

        # Get values needed next
        curr_action = db.execute('SELECT id FROM actions WHERE action = ?', 'buy')[0]['id']
        dt = datetime.datetime.utcnow()
        dt_date = dt.strftime("%Y-%m-%d")
        dt_time = dt.strftime("%H:%M:%S")

        # Insertion to transactions_utc
        db.execute('INSERT INTO transactions_utc (user_id, symbol, date, time, action_id, shares, price) VALUES (?, ?, ?, ?, ?, ?, ?)',
                   u_id, cur["symbol"], dt_date, dt_time, curr_action, shares, round(cur["price"], 2))

        # Update user's cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_money, u_id)

        # Redirect user to index.html
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        # Generate buy.html
        return render_template("/buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Get values needed next
    u_id = session["user_id"]
    dt = datetime.datetime.utcnow()
    dt_date = dt.strftime("%Y-%m-%d")
    dt_time = dt.strftime("%H:%M:%S")
    stocks = db.execute('SELECT transactions_utc.id, symbol, date, time, action, shares, price FROM transactions_utc '
                        'JOIN actions ON transactions_utc.action_id = actions.id '
                        'WHERE user_id = ? ORDER BY date ASC, time ASC', u_id)

    for row in stocks:
        row['value'] = usd(round(float(row['price']), 2) * int(row['shares']))
        if row['action'] == 'sell':
            row['value'] = '- '+row['value']

    return render_template("/history.html", stocks=stocks, method=["POST"])


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        # Ensure symbol exists database of iexapis.com
        if lookup(request.form.get("symbol")) == None:
            return apology("requested symbol not found in database", 400)

        # Aggregate values needed to show the  user
        output = lookup(request.form.get("symbol"))
        output["price"] = usd(round(output["price"], 2))

        # Redirect user to quoted.html to show result of request
        return render_template("/quoted.html", show=output, methods=["POST"])

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("/quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username is not empty
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure username is not existing
        elif len(db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))) > 0:
            return apology("username already exists", 400)

        # Ensure password is not empty
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure password confirmation is not empty
        elif not request.form.get("confirmation"):
            return apology("must provide password confirmation", 400)

        # Ensure password == confirmation
        elif not request.form.get("password") == request.form.get("confirmation"):
            return apology("confirmation doesn't match password", 400)

        # Send POST with correct values to /register
        else:
            name = request.form.get("username")
            passw = request.form.get("password")
            user_hash = generate_password_hash(passw)

            # Add user to table users
            db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", name, user_hash)

            # Get values needed for insertion to transactions_utc
            dt = datetime.datetime.utcnow()
            dt_date = dt.strftime("%Y-%m-%d")
            dt_time = dt.strftime("%H:%M:%S")
            default = db.execute("SELECT id, cash FROM users WHERE username = ?", name)
            action_id = db.execute("SELECT id FROM actions WHERE action = ?", 'initial')

            # Insert initial default deposit to table transactions_utc
            db.execute('INSERT INTO transactions_utc (user_id, symbol, date, time, action_id, shares, price) VALUES (?, ?, ?, ?, ?, ?, ?)',
                       default[0]['id'], "$", dt_date, dt_time, action_id[0]['id'], default[0]['cash'], 1.0)

            # Send new user to login
            return render_template("login.html")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        # Generate register.html
        return render_template("/register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must select symbol", 400)

        # Get values next needed
        cur_symbol = request.form.get("symbol")
        buy_id = db.execute('SELECT id FROM actions WHERE action = ?', 'buy')[0]['id']
        u_id = session['user_id']
        check = db.execute('SELECT SUM(CASE WHEN action_id = ? THEN shares ELSE shares * -1 END) AS sum FROM transactions_utc '
                           'WHERE user_id = ? AND symbol = ?', buy_id, u_id, cur_symbol)

        # Ensure cosen symbol exists in portfolio
        if len(check) == 0 or check[0]["sum"] == 0:
            return apology("symbol not in portfolio", 404)

        # Ensure number of shares to sell is positive integer and doesn't exceed the posessed shares
        if not request.form.get("shares") or request.form.get("shares") == "0" or request.form.get("shares").isdigit() == False:
            return apology("must provide a positive number of whole shares to sell", 400)
        elif int(request.form.get("shares")) > check[0]["sum"]:
            return apology("portfolio doesn't have enough shares to sell that amount", 400)

        # Get values next needed
        curr_action = db.execute('SELECT id FROM actions WHERE action = ?', 'sell')[0]['id']
        cur = lookup(cur_symbol)
        shares = int(request.form.get("shares"))
        money = db.execute('SELECT cash FROM users WHERE id = ?', u_id)
        new_money = float(money[0]['cash']) + (round(cur["price"], 2) * shares)
        dt = datetime.datetime.utcnow()
        dt_date = dt.strftime("%Y-%m-%d")
        dt_time = dt.strftime("%H:%M:%S")

        # Insertion to transactions_utc
        db.execute('INSERT INTO transactions_utc (user_id, symbol, date, time, action_id, shares, price) VALUES (?, ?, ?, ?, ?, ?, ?)',
                   u_id, cur["symbol"], dt_date, dt_time, curr_action, shares, round(cur["price"], 2))

        # Update user's cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_money, u_id)

        # Redirect user to index.html
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        # Get symbols of user's shares to select from on sell.html
        buy_id = db.execute('SELECT id FROM actions WHERE action = ?', 'buy')[0]['id']
        stocks = db.execute('SELECT symbol, SUM(CASE WHEN action_id = ? THEN shares ELSE shares * -1 END) AS sum FROM transactions_utc '
                            'WHERE user_id = ? GROUP BY symbol HAVING sum > 0', buy_id, session['user_id'])

        # Generate sell.html
        return render_template("/sell.html", stocks=stocks)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
