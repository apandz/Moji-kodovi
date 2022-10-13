package ba.etf.rma22.projekat.data.models

import androidx.room.ColumnInfo
import androidx.room.Entity
import androidx.room.PrimaryKey
import com.google.gson.annotations.SerializedName
import java.io.Serializable
import java.util.*

@Entity
data class Anketa(
    @PrimaryKey @ColumnInfo(name = "id") @SerializedName("id") val id: Int,
    @ColumnInfo(name = "naziv") @SerializedName("naziv") val naziv: String,
    @ColumnInfo(name = "datumPocetak") @SerializedName("datumPocetak") val datumPocetak: Date,
    @ColumnInfo(name = "datumKraj") @SerializedName("datumKraj") val datumKraj: Date?,
    @ColumnInfo(name = "trajanje") @SerializedName("trajanje") val trajanje: Int
): Serializable {
    override fun equals(other: Any?): Boolean {
        return id==(other as Anketa).id
    }
}
