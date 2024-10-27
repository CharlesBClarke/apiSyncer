# Use an official C++ build image
FROM ubuntu:22.04

# Install necessary packages
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    cmake \
    libmysqlcppconn-dev \
    libboost-all-dev \
    libssl-dev \
    libasio-dev \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /usr/src/app

# Copy the project files into the container
COPY . .

# Build the project
RUN mkdir build && \
    cd build && \
    cmake .. && \
    make

# Expose the port your app runs on
EXPOSE 18080

# Set the command to run the application
CMD ["./build/invmanr_api"]
