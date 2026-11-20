FROM gcc:14
WORKDIR /app
COPY NTA_lab2/*.cpp NTA_lab2/*.hpp ./
RUN g++ -std=c++17 -O2 -pthread *.cpp -o lab2
ENTRYPOINT ["./lab2"]
