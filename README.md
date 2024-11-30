# invmanr_api

`invmanr_api` is a C++ application that runs inside a Docker container and connects to a MySQL database. This guide provides instructions on how to build and run the Docker image.

## Prerequisites

- Docker installed on your system
- MySQL database running and accessible
- Environment variables for database credentials

## Build the Docker Image

 Build the Docker image using the following command:
   ```bash
   docker build -t invmanr_api .
   ```

   This command builds the Docker image and tags it as `invmanr_api`.

## Run the Docker Container

1. Ensure that your MySQL database is running and accessible.
   
2. Run the container with the required environment variables:
   ```bash
   docker run --rm \
   -e DB_HOST=<your-db-host> \
   -e DB_USER=<your-db-user> \
   -e DB_PASSWORD=<your-db-password> \
   -e DB_NAME=<your-db-name> \
   -p 18080:18080 \
   invmanr_api
   ```

   - Replace `<your-db-host>`, `<your-db-user>`, `<your-db-password>`, and `<your-db-name>` with your actual database credentials.
   - The `-p 18080:18080` flag maps the container port to the host, making the app accessible at `http://localhost:18080`.

3. After running, you can verify the service by visiting `http://localhost:18080` in your browser.

## Environment Variables

The application requires the following environment variables:

- `DB_HOST`: The hostname or IP address of the MySQL database.
- `DB_USER`: The username for the database connection.
- `DB_PASSWORD`: The password for the database connection.
- `DB_NAME`: The name of the database to connect to.

## Clean Up

To stop and remove the container, simply use `Ctrl+C` if it's running interactively or run:

```bash
docker container stop $(docker ps -q --filter ancestor=invmanr_api)
```

To remove the built Docker image:

```bash
docker rmi invmanr_api
```
