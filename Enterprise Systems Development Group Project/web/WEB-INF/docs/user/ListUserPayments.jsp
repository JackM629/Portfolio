<%-- 
    Document   : ListUserPayments
    Created on : 27-Nov-2017, 13:16:36
    Author     : Frazer, Sasha
--%>

<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ include file="/resources/UserDashboardNavBar.jsp" %>

<div class="content">
    <h1>Your Payments</h1>
    <table>
        <tr>
            <th>ID</th>
            <th>Member ID</th>
            <th>Type of Payment</th>
            <th>Amount</th>
            <th>Date</th>
            <th>Time</th>
        </tr>
        <c:forEach items="${data}" var="row" varStatus="rowStatus">
            <tr>
                <c:forEach items="${row}" var="column" varStatus="columnStatus">
                    <c:choose>
                        <c:when test="${columnStatus.last}">
                            <td>${column}</td>                           
                        </c:when>
                        <c:otherwise>
                            <td>${column}</td>
                        </c:otherwise>
                    </c:choose>
                </c:forEach>
            </tr>
        </c:forEach>
    </table>
</div>
