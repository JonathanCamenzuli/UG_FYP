FROM python:3.9-slim-buster

# Set the working directory to /app
WORKDIR /app

# Copy the current directory contents into the container at /app
COPY . /app

# Install any needed packages specified in requirements.txt
RUN pip install --trusted-host pypi.python.org -r requirements.txt

# Expose the CoAP port
EXPOSE 5683

# Set the working directory /app and specify the entrypoint
WORKDIR /app
ENTRYPOINT ["python", "server.py"]

# Mount the .env file
VOLUME ["/app/.env"]

# Run server.py when the container launches
CMD ["python", "server.py"]