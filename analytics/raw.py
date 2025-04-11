import pandas as pd


# FIRST: Read and process CSV file
def process_raw_data(csv_file_path):
    """
    Process the raw CSV file to extract relevant information.

    OUTPUT:
    - DataFrame with the following columns:
        - time
        - len
        - app
        - operation
        - valid
        - algorithm
    """

    columns = ["encoded", "time", "len"]

    # Read the CSV file
    raw_data = pd.read_csv(csv_file_path, header=None, names=columns)

    def extract_bits(encoded_value):
        """
        app | operation | valid |  alg
         0  |    0      |   00  | 0000
        """
        # Convert to binary and ensure 8 bits
        binary = format(encoded_value, "08b")

        # Extract bits from right to left
        app = int(binary[0], 2)  # 1st bit
        operation = int(binary[1], 2)  # 2nd bit
        valid = int(binary[2:4], 2)  # 3th and 4th bits
        algorithm = int(binary[4:], 2)  # last 4 bits

        return pd.Series([app, operation, valid, algorithm])

    def map_algorithm(alg_value):
        if alg_value == 0:
            return "NO_SIGN"
        elif alg_value == 1:
            return "RSA"
        elif alg_value == 2:
            return "ECDSA"
        elif alg_value == 3:
            return "EDDSA"
        else:
            return f"UNKNOWN({alg_value})"

    bit_columns = ["app_value", "operation_value", "valid_value", "algorithm_value"]
    raw_data[bit_columns] = raw_data["encoded"].apply(extract_bits)

    raw_data["app"] = raw_data["app_value"].apply(
        lambda x: "GroundControl" if x == 0 else "Autopilot"
    )
    raw_data["operation"] = raw_data["operation_value"].apply(
        lambda x: "SIGN" if x == 0 else "VERIFY"
    )
    raw_data["valid"] = raw_data["valid_value"].apply(
        lambda x: "INVALID" if x == 0 else "VALID" if x == 1 else "NA"
    )

    raw_data["algorithm"] = raw_data["algorithm_value"].apply(map_algorithm)

    raw_data["encodede bits"] = raw_data["encoded"].apply(lambda x: format(x, "08b"))

    # print(raw_data.head())
    # print(raw_data.to_string(index=False))

    # Drop the temporary value columns
    raw_data = raw_data.drop(
        [
            "app_value",
            "operation_value",
            "valid_value",
            "algorithm_value",
            "encodede bits",
            "encoded",
        ],
        axis=1,
    )

    # print(raw_data.to_string(index=False))

    return raw_data
