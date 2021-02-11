mkdir -p /tmp/www/YoupiBanane
echo youpi.bad_extension > /tmp/www/YoupiBanane/youpi.bad_extension
echo youpi.bla > /tmp/www/YoupiBanane/youpi.bla

mkdir -p /tmp/www/YoupiBanane/nop
echo youpi.bad_extension > /tmp/www/YoupiBanane/nop/youpi.bad_extension
echo other.pouic > /tmp/www/YoupiBanane/nop/other.pouic

mkdir -p /tmp/www/YoupiBanane/Yeah
echo not_happy.bad_extension > /tmp/www/YoupiBanane/Yeah/not_happy.bad_extension

#remove previous mistake
rm -rf /tmp/pages/pages
cp -R ../srcs/pages /tmp/
cp -R ../srcs/cgi-bin /tmp/

# /tmp
# ├── cgi-bin
# │   └── add.cgi
# ├── pages
# │   ├── error_page
# │   │   ├── 404.html
# │   │   └── 500.html
# │   └── good_page
# │       ├── add.html
# │       ├── bits-rain.html
# │       ├── color-quiz.html
# │       ├── colorclock.html
# │       ├── index.html
# │       └── magicWand.html
# └── www
#     └── YoupiBanane
#         ├── Yeah
#         │   └── not_happy.bad_extension
#         ├── nop
#         │   ├── other.pouic
#         │   └── youpi.bad_extension
#         ├── youpi.bad_extension
#         └── youpi.bla
