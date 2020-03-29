FROM ubuntu:18.04
COPY scripts /scripts
RUN "apt-get" "update"
RUN ["bash", "/scripts/install/install_apt_depends.sh"]
RUN ["bash", "/scripts/install/install_protobuf.sh"]
RUN ["bash", "/scripts/install/install_grpc.sh"]
RUN ["bash", "/scripts/install/install_boost.sh"]
RUN ["bash", "/scripts/install/install_hiredis.sh"]
RUN ["bash", "/scripts/install/install_libmongodbc.sh"]
RUN ["bash", "/scripts/install/install_libmongodb.sh"]
RUN ["bash", "/scripts/install/install_mvg.sh"]
RUN ["bash", "/scripts/install/install_mvs.sh"]
RUN ["bash", "/scripts/install/install_ctpl.sh"]
RUN ["bash", "/scripts/install/install_glog.sh"]
RUN ["bash", "/scripts/install/install_redis++.sh"]
RUN mkdir p /OnlineSFM
COPY CMakeLists.txt /OnlineSFM/CMakeLists.txt
COPY Makefile /OnlineSFM/Makefile
COPY common /OnlineSFM/common
COPY src /OnlineSFM/src
WORKDIR /OnlineSFM
CMD ["make", "build"]