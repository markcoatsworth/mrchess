# Stage 1: compile the move engine with g++
FROM debian:trixie-slim AS build

RUN apt-get update \
    && apt-get install -y --no-install-recommends g++ cmake make \
    && rm -rf /var/lib/apt/lists/*

COPY src /src
RUN cmake -S /src -B /build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build /build -j"$(nproc)"

# Stage 2: nginx serves the static frontend and hands CGI requests to fcgiwrap
FROM debian:trixie-slim

RUN apt-get update \
    && apt-get install -y --no-install-recommends nginx fcgiwrap \
    && rm -rf /var/lib/apt/lists/* /etc/nginx/sites-enabled/default

COPY deploy/nginx.conf /etc/nginx/conf.d/mrchess.conf.template
COPY deploy/entrypoint.sh /entrypoint.sh

COPY index.html favicon.ico /var/www/mrchess/
COPY css /var/www/mrchess/css
COPY images /var/www/mrchess/images
COPY js /var/www/mrchess/js
COPY --from=build /build/mrchess /var/www/mrchess/cgi-bin/mrchess.cgi

# Cloud Run sets PORT at runtime; 8080 is its default
ENV PORT=8080
EXPOSE 8080

ENTRYPOINT ["/entrypoint.sh"]
