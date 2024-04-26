import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
import re

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    portfolio_data = []
    portfolios = db.execute("SELECT * FROM ownes WHERE user_id = ?", session["user_id"])
    user_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
    grand_total = 0

    for portfolio in portfolios:
        symbol = portfolio["symbol"]
        shares = portfolio["shares"]
        price_info = lookup(symbol)
        current_price = price_info["price"]
        total_value = current_price * shares
        grand_total += total_value
        portfolio_data.append({
            "symbol": symbol,
            "shares": shares,
            "current_price": current_price,
            "total_value": total_value})
    grand_total += user_cash

    # return render_template("index.html", price_info=price_info)
    return render_template("index.html", portfolio_data=portfolio_data, usd=usd, user_cash=usd(user_cash), grand_total=usd(grand_total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol").upper()
        if not symbol:
            return apology("Input a stock", 400)
        try:
            shares = float(request.form.get("shares"))
        except ValueError:
            return apology("Shares must be a valid number", 400)

        if shares < 0 or shares % 1 != 0:
            return apology("Shares must be a whole number and cannot be negative", 400)

        stock = lookup(symbol)
        if stock == None:
            return apology("Stock not found", 400)
        if request.form.get("buy"):

            user_balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
            balance = user_balance[0]["cash"]
            total_cost = stock["price"] * int(shares)

            if balance < total_cost:
                return apology("Not enought cash.. sadge", 400)

            new_balance = balance - total_cost
            db.execute("INSERT INTO transactions (user_id, symbol, shares, type, price) VALUES (?, ?, ?, ?, ?)", session["user_id"], stock["symbol"], int(shares), "bought", stock["price"])
            db.execute("UPDATE users SET cash= ? WHERE id= ?", new_balance, session["user_id"])

            stock_exist = db.execute("SELECT * FROM ownes WHERE user_id = ? AND symbol = ?", session["user_id"], symbol)

            if stock_exist:
                updated_shares = stock_exist[0]["shares"] + int(shares)
                db.execute("UPDATE ownes SET shares = ? WHERE user_id = ? AND symbol = ?", updated_shares, session["user_id"], symbol)
            else:
                db.execute("INSERT INTO ownes (user_id, symbol, shares) VALUES (?, ?, ?)", session["user_id"], symbol, shares)
            flash(f"Bought {int(shares)} of {symbol} for a total of {usd(total_cost)}")
            app.logger.info(usd(total_cost))
            return redirect("/")
    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute("SELECT symbol, shares, type, price, transaction_date FROM transactions WHERE user_id = ? ORDER BY transaction_date DESC", session["user_id"])
    return render_template("history.html", transactions=transactions)

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
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
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
    if request.method == "POST":
        search = request.form.get("symbol")
        stocks = lookup(search)
        app.logger.info(stocks)
        if not search:
            return apology("Input a stock", 400)
        if stocks == None:
            return apology("Quote not found", 400)
        return render_template("quote.html", stocks=stocks, usd=usd)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        user_name= request.form.get("username")
        password= request.form.get("password")
        re_password= request.form.get("confirmation")

        """Check for blank username"""
        if not user_name:
            return apology("must provide username", 400)

        if db.execute("SELECT username FROM users WHERE  username = ?", user_name):
            return apology("Username already exist", 400)

        """Check for password match"""
        if not password == re_password:
            return apology("Password doesn't match", 400)

        pattern = re.compile(r"^(?=.*[A-Za-z])(?=.*\d)(?=.*[@$!%*?&])[A-Za-z\d@$!%*?&]{6,}$")
        if not re.match(pattern, password):
            return apology("Passowrd must have atlease 6 letters, numbers and/or symbols ", 400)
        else:
            password_hash = generate_password_hash(password)
            db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", user_name, password_hash)
            return render_template("login.html")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    portfolios = db.execute("SELECT * FROM ownes WHERE user_id = ?", session["user_id"])
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        if request.form.get("sell"):
            if shares == "" or int(shares) < 1:
                return apology("Need to sell at least 1 stock", 400)
            current_shares = db.execute("SELECT shares FROM ownes WHERE user_id = ? AND symbol = ?", session["user_id"], symbol)
            owned_shares = current_shares[0]["shares"]
            app.logger.info(owned_shares)

            if int(shares) > owned_shares:
                return apology("Cannot sell more shares than you own", 400)
            stock = lookup(symbol)
            user_balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
            balance = user_balance[0]["cash"]
            total_cost = stock["price"] * int(shares)
            new_balance = balance + total_cost
            db.execute("INSERT INTO transactions (user_id, symbol, shares, type, price) VALUES (?, ?, ?, ?, ?)", session["user_id"], stock["symbol"], shares, "sold", stock["price"])
            db.execute("UPDATE users SET cash= ? WHERE id= ?", new_balance, session["user_id"])

            update_ownes = db.execute("SELECT * FROM ownes WHERE user_id = ? AND symbol = ?", session["user_id"], symbol)
            updated_shares = update_ownes[0]["shares"] - int(shares)

            if updated_shares < 1:
                db.execute("DELETE FROM ownes WHERE symbol = ?", symbol)
            else:
                db.execute("UPDATE ownes SET shares = ? WHERE user_id = ? AND symbol = ?", updated_shares, session["user_id"], symbol)

            flash("Sold")
            return redirect("/")

    return render_template("sell.html", portfolios=portfolios)
