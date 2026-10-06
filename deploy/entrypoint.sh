#!/bin/sh
set -e

# Listen on the port Cloud Run assigns
sed "s/__PORT__/${PORT}/" /etc/nginx/conf.d/mrchess.conf.template > /etc/nginx/conf.d/mrchess.conf

# fcgiwrap runs the CGI binary; -c sets how many requests it can run at once
setpriv --reuid=www-data --regid=www-data --init-groups \
    fcgiwrap -s tcp:127.0.0.1:9000 -c "${FCGI_CHILDREN:-$(( $(nproc) * 2 ))}" &

exec nginx -g 'daemon off;'
