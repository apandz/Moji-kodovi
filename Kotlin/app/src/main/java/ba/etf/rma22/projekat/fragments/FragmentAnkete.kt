package ba.etf.rma22.projekat.fragments

import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.AdapterView
import android.widget.Spinner
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.GridLayoutManager
import androidx.recyclerview.widget.RecyclerView
import ba.etf.rma22.projekat.MainActivity
import ba.etf.rma22.projekat.R
import ba.etf.rma22.projekat.data.models.Anketa
import ba.etf.rma22.projekat.help.RecyclerItemDecoration
import ba.etf.rma22.projekat.view.AnketaListAdapter
import ba.etf.rma22.projekat.viewmodel.AnketaViewModel
import ba.etf.rma22.projekat.viewmodel.PitanjeViewModel
import java.util.*

class FragmentAnkete : Fragment() {
    private lateinit var ankete: RecyclerView
    private lateinit var anketeAdapter: AnketaListAdapter
    private lateinit var filter: Spinner
    private var anketaViewModel = AnketaViewModel()
    private var pitanjeViewModel = PitanjeViewModel()

    override fun onCreate(savedInstanceState: Bundle?) {
        anketaViewModel.getAll(
            onSuccess = ::onSuccess,
            onError = ::onError
        )
        super.onCreate(savedInstanceState)
    }

    private fun onSuccess(ankete:List<Anketa>){
        /*val toast = Toast.makeText(context, "Upcoming movies found", Toast.LENGTH_SHORT)
        toast.show()*/
        anketeAdapter.updateAnkete(ankete)
    }
    fun onError() {
        /*val toast = Toast.makeText(context, "Search error", Toast.LENGTH_SHORT)
        toast.show()*/
    }


    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        var view = inflater.inflate(R.layout.fragment_ankete, container, false)
        ankete=view.findViewById(R.id.listaAnketa)
        ankete.layoutManager= GridLayoutManager(
            activity,
            2
        )

        ankete.addItemDecoration(RecyclerItemDecoration(2,15))
        anketeAdapter= AnketaListAdapter(listOf())
        ankete.adapter=anketeAdapter
        filter=view.findViewById(R.id.filterAnketa)
        filter.onItemSelectedListener = object : AdapterView.OnItemSelectedListener {
            override fun onItemSelected(
                parent: AdapterView<*>,
                view: View,
                position: Int,
                id: Long
            ) {
                val odabrano=parent.getItemAtPosition(position).toString()
                when (odabrano) {
                    "Sve ankete" -> anketaViewModel.getAll(onSuccess = ::onSuccess, onError = ::onError)
                    "Urađene ankete" -> anketaViewModel.getDone(onSuccess = ::onSuccess, onError = ::onError)
                    "Buduće ankete" -> anketaViewModel.getFuture(onSuccess = ::onSuccess, onError = ::onError)
                    "Prošle ankete" -> anketaViewModel.getNotTaken(onSuccess = ::onSuccess, onError = ::onError)
                    else -> anketaViewModel.getMyAnkete(onSuccess = ::onSuccess, onError = ::onError)
                }
            }

            override fun onNothingSelected(parent: AdapterView<*>?) {
                anketaViewModel.getMyAnkete(onSuccess = ::onSuccess, onError = ::onError)
            }
        }

        anketeAdapter.onItemClick = { anketa ->
            val trenutniDatum = Calendar.getInstance().time
            if(/*anketaViewModel.getMyAnkete(onSuccess = ::onSuccess, onError = ::onError).contains(anketa) &&*/ !anketa.datumPocetak.after(trenutniDatum)) {
                val pitanja = pitanjeViewModel.getPitanja(anketa.naziv, "naziv")
                for (i in 0..pitanja.size - 1) {
                    val bundle = Bundle()
                    bundle.putSerializable("pitanje", pitanja[i])
                    bundle.putSerializable("anketa",anketa)

                    val fragmentPitanje = FragmentPitanje()
                    fragmentPitanje.arguments = bundle

                    if (i == 0 || i == 1) MainActivity.viewPagerAdapter.refreshFragment(
                        i,
                        fragmentPitanje
                    )
                    else MainActivity.viewPagerAdapter.add(i, fragmentPitanje)
                }
                val bundle = Bundle()
                bundle.putSerializable("anketa", anketa)

                val fragmentPredaj = FragmentPredaj()
                fragmentPredaj.arguments = bundle

                MainActivity.viewPagerAdapter.add(pitanja.size, fragmentPredaj)
            }
        }

        return view
    }

    override fun onResume() {
        if(MainActivity.viewPagerAdapter.fragments[1] is FragmentPoruka) {
            Handler(Looper.getMainLooper()).postDelayed({
                MainActivity.viewPagerAdapter.refreshFragment(1, FragmentIstrazivanje())
                                                        }, 50)
        }
        when (filter.selectedItem.toString()) {
            "Sve ankete" -> anketaViewModel.getAll(onSuccess = ::onSuccess, onError = ::onError)
            "Urađene ankete" -> anketaViewModel.getDone(onSuccess = ::onSuccess, onError = ::onError)
            "Buduće ankete" -> anketaViewModel.getFuture(onSuccess = ::onSuccess, onError = ::onError)
            "Prošle ankete" -> anketaViewModel.getNotTaken(onSuccess = ::onSuccess, onError = ::onError)
            else -> anketaViewModel.getMyAnkete(onSuccess = ::onSuccess, onError = ::onError)
        }
        super.onResume()
    }

    companion object {
        fun newInstance() = FragmentAnkete()
    }
}