FROM ubuntu:18.04
COPY get_depends.sh /get_depends.sh
RUN "apt-get" "update"
CMD ["./get_depends.sh"]