package ba.etf.rma22.projekat.fragments

import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.TextView
import androidx.fragment.app.Fragment
import ba.etf.rma22.projekat.MainActivity
import ba.etf.rma22.projekat.R
import ba.etf.rma22.projekat.data.models.Anketa
import ba.etf.rma22.projekat.interfaces.Communicator
import ba.etf.rma22.projekat.interfaces.Progres
import ba.etf.rma22.projekat.viewmodel.PitanjeViewModel
import java.util.*

class FragmentPredaj : Fragment(), Progres {
    private lateinit var progresTekst: TextView
    private lateinit var dugmePredaj: Button
    private var pitanjeViewModel = PitanjeViewModel()

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_predaj, container, false)

        val anketa = requireArguments().getSerializable("anketa") as Anketa

        progresTekst=view.findViewById(R.id.progresTekst)
        dugmePredaj=view.findViewById(R.id.dugmePredaj)

        val trenutniDatum: Date = Calendar.getInstance().time
        var anketaProgres=0.0F

        if(anketa.datumKraj!=null && anketa.datumKraj.before(trenutniDatum)) {
            dugmePredaj.isEnabled=false
            anketaProgres= pitanjeViewModel.getOdgovori(anketa.naziv,"naziv").size*1.0F/pitanjeViewModel.getPitanja(anketa.naziv,"naziv").size
        }

        progresTekst.setText(noviProgres(anketaProgres).toString()+"%")

        dugmePredaj.setOnClickListener { _ ->
            /*anketa.datumRada=trenutniDatum
            Kada postavim datumRada na trenutniDatum, anketa će imati plavi krug.
            Međutim, sada ne mogu promijeniti datumRada, jer uzimam ankete
            iz AnketeStaticData. Kada budem uzimala ankete iz baze podataka, završene
            ankete će imati plavi krug.*/

            Handler(Looper.getMainLooper()).postDelayed({
                MainActivity.viewPagerAdapter.defaultFragmenti()
                MainActivity.viewPager.setCurrentItem(1)
            }, 100)
            val communicator=activity as Communicator
            communicator.passDataCom("Završili ste anketu "+anketa.naziv+" u okviru istraživanja "+"naziv")
        }

        return view
    }

    override fun onResume() {
        val anketa = requireArguments().getSerializable("anketa") as Anketa
        val trenutniDatum: Date = Calendar.getInstance().time
        if(anketa.datumKraj!=null && !anketa.datumKraj.before(trenutniDatum)) {
            val anketaProgres= pitanjeViewModel.getOdgovori(anketa.naziv,"naziv").size*1.0F/pitanjeViewModel.getPitanja(anketa.naziv,"naziv").size
            progresTekst.setText(noviProgres(anketaProgres).toString()+"%")
        }
        super.onResume()
    }

    companion object {
        fun newInstance() = FragmentPitanje()
    }
}