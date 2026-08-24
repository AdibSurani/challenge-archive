FROM python:3.12-slim
WORKDIR /app

RUN useradd -m chall
RUN pip install fastecdsa
RUN apt update
RUN apt install -y socat

COPY --chmod=444 baby-shark.py .

USER chall

ENV FLAG="oiccflag{Number_on3_m0st_watched_youtuB3_vide0}"
ENTRYPOINT ["socat","TCP-LISTEN:1337,reuseaddr,fork","EXEC:python3 -u baby-shark.py"]
