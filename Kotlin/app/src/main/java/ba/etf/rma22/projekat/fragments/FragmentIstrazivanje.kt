package ba.etf.rma22.projekat.fragments

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.AdapterView
import android.widget.ArrayAdapter
import android.widget.Button
import android.widget.Spinner
import androidx.fragment.app.Fragment
import ba.etf.rma22.projekat.R
import ba.etf.rma22.projekat.interfaces.Communicator
import ba.etf.rma22.projekat.viewmodel.GrupaViewModel
import ba.etf.rma22.projekat.viewmodel.IstrazivanjeViewModel

class FragmentIstrazivanje : Fragment() {
    private lateinit var odabirGodina: Spinner
    private lateinit var odabirIstrazivanja: Spinner
    private lateinit var odabirGrupa: Spinner
    private lateinit var dodajIstrazivanjeDugme: Button
    private var istrazivanjeViewModel = IstrazivanjeViewModel()
    private var grupaViewModel = GrupaViewModel()
    private lateinit var communicator: Communicator

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        var view = inflater.inflate(R.layout.fragment_istrazivanje, container, false)

        communicator = activity as Communicator

        odabirGodina=view.findViewById(R.id.odabirGodina)
        odabirIstrazivanja=view.findViewById(R.id.odabirIstrazivanja)
        odabirGrupa=view.findViewById(R.id.odabirGrupa)
        dodajIstrazivanjeDugme=view.findViewById(R.id.dodajIstrazivanjeDugme)

        var odabranaGodina: String?
        var odabranoIstrazivanje: String?
        val context=this.requireActivity()

        if(istrazivanjeViewModel.getUpisanaIstrazivanja().size>3) {
            val godina = istrazivanjeViewModel.getZadnjaGodina()
            odabranaGodina = godina
            odabirGodina.setSelection(odabranaGodina.toInt())
            val adapter =
                ArrayAdapter(context, android.R.layout.simple_spinner_dropdown_item, listOf("")+istrazivanjeViewModel.getIstrazivanjeByGodina(
                    odabranaGodina.toInt()))
            odabirIstrazivanja.adapter = adapter
        }

        odabirGodina.onItemSelectedListener = object : AdapterView.OnItemSelectedListener {
            override fun onItemSelected(
                parent: AdapterView<*>,
                view: View,
                position: Int,
                id: Long
            ) {
                dodajIstrazivanjeDugme.isEnabled = !odabirGodina.selectedItem.toString().equals("") &&
                        !odabirIstrazivanja.selectedItem.toString().equals("") &&
                        !odabirGrupa.selectedItem.toString().equals("")

                odabirGrupa.adapter=
                    ArrayAdapter(context, android.R.layout.simple_spinner_dropdown_item,listOf(""))
                odabranaGodina = odabirGodina.selectedItem as String
                if(odabranaGodina.equals(""))
                    odabranaGodina="0"
                val adapter =
                    ArrayAdapter(context, android.R.layout.simple_spinner_dropdown_item, listOf("")+istrazivanjeViewModel.getIstrazivanjeByGodina(
                        odabranaGodina!!.toInt()))
                odabirIstrazivanja.adapter = adapter

                odabirIstrazivanja.onItemSelectedListener =
                    object : AdapterView.OnItemSelectedListener {
                        override fun onItemSelected(
                            parent: AdapterView<*>,
                            view: View,
                            position: Int,
                            id: Long
                        ) {
                            dodajIstrazivanjeDugme.isEnabled =
                                !odabirGodina.selectedItem.toString().equals("") &&
                                        !odabirIstrazivanja.selectedItem.toString().equals("") &&
                                        !odabirGrupa.selectedItem.toString().equals("")

                            odabranoIstrazivanje = odabirIstrazivanja.selectedItem.toString()
                            val adapter1 = ArrayAdapter(
                                context,
                                android.R.layout.simple_spinner_dropdown_item,
                                listOf("")+grupaViewModel.getGroupsByIstrazivanje(odabranoIstrazivanje!!)
                            )
                            odabirGrupa.adapter = adapter1

                            odabirGrupa.onItemSelectedListener = object : AdapterView.OnItemSelectedListener {
                                override fun onItemSelected(
                                    parent: AdapterView<*>,
                                    view: View,
                                    position: Int,
                                    id: Long
                                ) {
                                    dodajIstrazivanjeDugme.isEnabled = !odabirGodina.selectedItem.toString().equals("") &&
                                            !odabirIstrazivanja.selectedItem.toString().equals("") &&
                                            !odabirGrupa.selectedItem.toString().equals("")
                                }
                                override fun onNothingSelected(parent: AdapterView<*>) {

                                }
                            }
                        }
                        override fun onNothingSelected(parent: AdapterView<*>) {

                        }
                    }
            }
            override fun onNothingSelected(parent: AdapterView<*>) {

            }
        }

        dodajIstrazivanjeDugme.isEnabled = !odabirGodina.selectedItem.toString().equals("") &&
                !odabirIstrazivanja.selectedItem.toString().equals("") &&
                !odabirGrupa.selectedItem.toString().equals("")

        dodajIstrazivanjeDugme.setOnClickListener {
            istrazivanjeViewModel.setZadnjaGodina(odabirGodina.selectedItem.toString())
            istrazivanjeViewModel.upisiIstrazivanje(odabirIstrazivanja.selectedItem.toString(),odabirGrupa.selectedItem.toString())
            communicator.passDataCom("Uspješno ste upisani u grupu "+odabirGrupa.selectedItem.toString()+" istraživanja "+odabirIstrazivanja.selectedItem.toString()+"!")
        }
        return view
    }

    companion object {
        fun newInstance() = FragmentIstrazivanje()
    }
}