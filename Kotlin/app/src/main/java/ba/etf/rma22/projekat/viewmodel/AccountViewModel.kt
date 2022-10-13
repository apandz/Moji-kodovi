package ba.etf.rma22.projekat.viewmodel

import ba.etf.rma22.projekat.data.repositories.AccountRepository
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.launch

class AccountViewModel {
    private val scope = CoroutineScope(Job() + Dispatchers.Main)

    fun postaviHash(payload : String) {
        scope.launch{
            AccountRepository.postaviHash(payload)
        }
    }
}