import pandas as pd

# Usar com venv:
# source venv/bin/activate
# Para desativar:
# deactivate
# import seaborn as sn

import matplotlib.pyplot as plt


def correlation_analysis(data):

    def map_algorithm(alg_value):
        if alg_value == "NO_SIGN":
            return 0
        elif alg_value == "RSA":
            return 1
        elif alg_value == "ECDSA":
            return 2
        elif alg_value == "EDDSA":
            return 3
        else:
            return f"UNKNOWN({alg_value})"

    qgc_verify = data["GroundControl"]["verify"].copy()
    # print(qgc_verify.head())
    # qgc_verify["app"] = qgc_verify["app"].apply(
    #     lambda x: 0 if x == "GroundControl" else 1
    # )
    # qgc_verify["valid"] = qgc_verify["valid"].apply(
    #     lambda x: 0 if x == "INVALID" else 1
    # )
    # qgc_verify["algorithm"] = qgc_verify["algorithm"].apply(map_algorithm)

    df_analysis = qgc_verify[qgc_verify["algorithm"] == "RSA"].drop(
        ["app", "valid", "algorithm"], axis=1
    )
    correlation = df_analysis.corr()
    print(correlation)

    # df_analysis = qgc_verify[qgc_verify["algorithm"] == "RSA"].drop(
    #     ["app", "valid", "algorithm"], axis=1
    # )
    # correlation = df_analysis.corr()
    # print(correlation)

    # df_analysis = qgc_verify[qgc_verify["algorithm"] == "RSA"].drop(
    #     ["app", "valid", "algorithm"], axis=1
    # )
    # correlation = df_analysis.corr()
    # print(correlation)

    # df_analysis = qgc_verify[qgc_verify["algorithm"] == "RSA"].drop(
    #     ["app", "valid", "algorithm"], axis=1
    # )
    # correlation = df_analysis.corr()
    # print(correlation)
    # plot = sn.heatmap(correlation, annot=True, fmt=".1f", linewidths=0.6)
    # plt.show()
