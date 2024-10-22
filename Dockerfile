# Use an official C++ build image
FROM ubuntu:22.04

# Install necessary packages
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    cmake \
    libmysqlcppconn-dev \
    libssl-dev \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /usr/src/app

# Copy the project files into the container
COPY . .

# Build the project
RUN cmake -B build . && \
    cmake --build build

# Expose the port your app runs on
EXPOSE 8080

# Set the command to run the application
CMD ["./build/invmanr_api"]
