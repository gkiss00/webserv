mkdir -p /tmp/www/YoupiBanane
touch /tmp/www/YoupiBanane/youpi.bad_extension
touch /tmp/www/YoupiBanane/youpi.bla

mkdir -p /tmp/www/YoupiBanane/nop
touch /tmp/www/YoupiBanane/nop/youpi.bad_extension
touch /tmp/www/YoupiBanane/nop/other.pouic

mkdir -p /tmp/www/YoupiBanane/Yeah
touch /tmp/www/YoupiBanane/Yeah/not_happy.bad_extension

cp -R srcs/pages /tmp/pages

# /tmp
# ├── pages
# │   ├── cgi-bin
# │   │   ├── add.cgi
# │   │   └── add.html
# │   ├── error_page
# │   │   ├── 404.html
# │   │   └── 500.html
# │   └── good_page
# │       ├── bits-rain.html
# │       ├── color-quiz.html
# │       ├── colorclock.html
# │       └── index.html
# └── www
#     └── YoupiBanane
#         ├── Yeah
#         │   └── not_happy.bad_extension
#         ├── nop
#         │   ├── other.pouic
#         │   └── youpi.bad_extension
#         ├── youpi.bad_extension
#         └── youpi.bla