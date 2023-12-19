@echo off
echo Git push

set /p commit_txt=Commit text: 

git init
git add . --all
git commit -m "%commit_txt%"
git branch -M main
git remote add origin https://github.com/leonllrtest/CFPT_robot_strat_firmware_MK1.git
git push -u origin main


pause