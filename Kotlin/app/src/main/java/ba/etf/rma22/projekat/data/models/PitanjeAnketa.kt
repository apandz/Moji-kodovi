package ba.etf.rma22.projekat.data.models

import com.google.gson.annotations.SerializedName

data class PitanjeAnketa(
    @SerializedName("naziv") val naziv: String,
    @SerializedName("anketa") val anketa: String,
    @SerializedName("istrazivanje") val istrazivanje: String
) {
    override fun equals(other: Any?): Boolean {
        return (other as PitanjeAnketa).naziv == naziv && other.anketa == anketa
                && other.istrazivanje == istrazivanje
    }
}
