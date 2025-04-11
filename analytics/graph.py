import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

markers = {"NO_SIGN": "o", "RSA": "s", "ECDSA": "^", "EDDSA": "D"}

colors = {"NO_SIGN": "blue", "RSA": "red", "ECDSA": "green", "EDDSA": "purple"}


def graph_generator(data):
    """
    Generate graphs for the given data.
    Parameters:
    data (dict): A dictionary containing the split data for Autopilot and GroundControl.
    The structure of the dictionary is as follows:
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
    Returns:
    None
    """
    for app in data.keys():
        for op in data[app].keys():
            operation = data[app][op]

            # Prepare the data for plotting
            algorithms = ["NO_SIGN", "RSA", "ECDSA", "EDDSA"]
            plt.figure(figsize=(10, 6))

            for alg in algorithms:
                alg_data = operation[operation["algorithm"] == alg]

                if not alg_data.empty:
                    plt.plot(
                        alg_data["len"],
                        alg_data["time"],
                        marker=markers[alg],
                        color=colors[alg],
                        label=alg,
                        linestyle="",
                    )

            # Customize the plot
            plt.title(f"Time performance - {app} - {op}")
            plt.xlabel("Message Size (bytes)")
            plt.ylabel("Time (μs)")
            plt.legend()
            plt.grid(True)

    plt.show()


def normalized_graph_generator(data):
    """
    Generate normalized graphs for the given data.
    Parameters:
    data (dict): A dictionary containing the split data for Autopilot and GroundControl.
    The structure of the dictionary is as follows:
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
    Returns:
    None
    """
    for app in data.keys():
        for op in data[app].keys():
            operation = data[app][op]

            operation["normalized"] = (operation["time"] - operation["time"].min()) / (
                operation["time"].max() - operation["time"].min()
            )

            # Prepare the data for plotting
            algorithms = ["NO_SIGN", "RSA", "ECDSA", "EDDSA"]
            plt.figure(figsize=(10, 6))

            for alg in algorithms:
                alg_data = operation[operation["algorithm"] == alg]

                if not alg_data.empty:
                    plt.plot(
                        alg_data["len"],
                        alg_data["normalized"],
                        marker=markers[alg],
                        color=colors[alg],
                        label=alg,
                        linestyle="",
                    )

            # Customize the plot
            plt.title(f"Normalized Time Performance - {app} - {op}")
            plt.xlabel("Message Size (bytes)")
            plt.ylabel("Normalized Time")
            plt.legend()
            plt.grid(True)

    plt.show()


def boxplot_generator(data):
    # ! WIP
    """
    Generate a boxplot for the GroundControl verify operation based on the time.
    Parameters:
    data (dict): A dictionary containing the split data for Autopilot and GroundControl.
    Returns:
    None
    """
    for app in data.keys():
        for op in data[app].keys():
            operation = data[app][op]

            if not operation.empty:
                # operation.boxplot(column="time", by="algorithm", grid=True)
                sns.violinplot(x="time", y="algorithm", data=operation)

                # ground_control_verify = data["GroundControl"]["verify"]

                # ground_control_verify.boxplot(column="time", by="algorithm", grid=True)

                # Customize the plot
                plt.title(f"Boxplot of Time - {app} - {op}")
                plt.suptitle("")  # Remove the default Pandas title
                plt.xlabel("Algorithm")
                plt.ylabel("Time (μs)")
                plt.grid(True)

    plt.show()
