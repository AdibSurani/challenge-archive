FROM ghcr.io/downunderctf/docker-vendor/nsjail@sha256:a0853d266c92078564ba7d864139c9b19a816ff81b9d10fa63e02d105e420a00
WORKDIR /home/ctf/chal

RUN rm /home/ctf/chal/*

ENV JAIL_CWD=/chal
ENV EXEC_BIN=/chal/ent-rop-y

COPY --chmod=755 ent-rop-y .
COPY --chmod=444 flag.txt .

EXPOSE 1337/tcp
