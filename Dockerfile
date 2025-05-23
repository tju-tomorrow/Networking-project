# base image from gradescope
FROM gradescope/auto-builds:ubuntu-18.04

# make necessary directories
RUN apt-get update &&\
    apt-get -y install gcc flex bison build-essential siege apache2-utils libssl-dev &&\
    # change ApacheBench request HTTP version to 1.1
    perl -pi -e 's/HTTP\/1.0/HTTP\/1.1/g' /usr/bin/ab

WORKDIR /home
