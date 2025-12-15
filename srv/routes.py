from flask import Blueprint, render_template, redirect, url_for, request
from flask_login import login_user, logout_user, login_required
from .models import User
from . import db
import can
import json
import time


main = Blueprint("main", __name__)

@main.route("/")
def index():
    new_user_button = "New user"
    return render_template('index.html',new_user_button=new_user_button)


@main.route('/submit', methods=['POST'])
def submit():
    if request.method == 'POST':
        name = request.form['name']
    
    # Perform some processing with the form data
    return 'Hello, ' + name + '!'


@main.route("/login", methods=["GET", "POST"])
def login():
    if request.method == "POST":
        user = User.query.filter_by(
            username=request.form["username"]
        ).first()

        if user:
            login_user(user)
            return redirect(url_for("main.index"))

    return render_template("login.html")

@main.route("/logout")
@login_required
def logout():
    logout_user()
    return redirect(url_for("main.login"))