def split_data(raw_data):
    """
    Split the raw data into two main categories: Autopilot and GroundControl.
    Each category is further divided into two operations: SIGN and VERIFY.
    The function returns a dictionary containing the split data.
    The dictionary structure is as follows:
    {
        'Autopilot': {
            'sign': DataFrame,
            'verify': DataFrame
        },
        'GroundControl': {
            'sign': DataFrame,
            'verify': DataFrame
        }
    }
    The DataFrames contain the relevant data for each category and operation.
    Parameters:
    raw_data (DataFrame): The raw data DataFrame containing the processed information.
    Returns:
    dict: A dictionary containing the split data for Autopilot and GroundControl.
    """

    data = {
        "Autopilot": {
            "sign": None,
            "verify": None,
        },
        "GroundControl": {
            "sign": None,
            "verify": None,
        },
    }

    # Filter the raw data for Autopilot and GroundControl
    # and separate them into SIGN and VERIFY operations

    # Organize the data Autopilot sign
    data["Autopilot"]["sign"] = raw_data[
        (raw_data["app"] == "Autopilot") & (raw_data["operation"] == "SIGN")
    ]
    data["Autopilot"]["sign"] = data["Autopilot"]["sign"].drop(
        ["operation", "valid"], axis=1
    )

    # Organize the data Autopilot verify
    data["Autopilot"]["verify"] = raw_data[
        (raw_data["app"] == "Autopilot") & (raw_data["operation"] == "VERIFY")
    ]
    data["Autopilot"]["verify"] = data["Autopilot"]["verify"].drop(
        ["operation"], axis=1
    )

    # Organize the data GroundControl sign
    data["GroundControl"]["sign"] = raw_data[
        (raw_data["app"] == "GroundControl") & (raw_data["operation"] == "SIGN")
    ]
    data["GroundControl"]["sign"] = data["GroundControl"]["sign"].drop(
        ["operation", "valid"], axis=1
    )

    # Organize the data GroundControl verify
    data["GroundControl"]["verify"] = raw_data[
        (raw_data["app"] == "GroundControl") & (raw_data["operation"] == "VERIFY")
    ]
    data["GroundControl"]["verify"] = data["GroundControl"]["verify"].drop(
        ["operation"], axis=1
    )

    # print("Autopilot SIGN: ", data["Autopilot"]["sign"].head())
    # print("Autopilot VERIFY: ", data["Autopilot"]["verify"].head())
    # print("GroundControl SIGN: ", data["GroundControl"]["sign"].head())
    # print("GroundControl VERIFY: ", data["GroundControl"]["verify"].head())

    return data
